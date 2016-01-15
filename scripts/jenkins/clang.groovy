node('docker')
{
	// Checks out into subdirectory ogs
	stage 'Checkout'
	checkout([$class: 'GitSCM',
		branches: [[name: '*/master']],
		doGenerateSubmoduleConfigurations: false,
		extensions:
			[[$class: 'RelativeTargetDirectory', relativeTargetDir: 'ogs']],
		submoduleCfg: [],
		userRemoteConfigs:
			[[url: 'https://github.com/ufz/ogs']]])


	// Multiple configurations are build in parallel
	parallel linux: {
		docker.image('ogs6/clang-ogs-base:latest').inside
		{
			build 'build', '-DOGS_ADDRESS_SANITIZER=ON -DOGS_UNDEFINED_BEHAVIOR_SANITIZER=ON -DBOOST_ROOT=$BOOST_ROOT', ''

			stage 'Unit tests'
			sh '''cd build
			      rm -rf tests/testrunner.xml
			      bin/testrunner --gtest_output=xml:./tests/testrunner.xml'''

			stage 'End-to-end tests'
			sh '''cd build
			      make ctest'''
		}
		step([$class: 'LogParserPublisher', failBuildOnError: true, unstableOnWarning: true,
			projectRulePath: 'ogs/scripts/jenkins/clang-log-parser.rules', useProjectRule: true])
	}

	//linux_gui: {
	//	docker.image('ogs6/gcc-ogs-gui:latest').inside {
	//		build 'build_gui', '-DOGS_BUILD_GUI=ON -DOGS_BUILD_TESTS=OFF -DOGS_BUILD_CLI=OFF', 'package'
	//	}
	//}

	// end parallel

	step([$class: 'JUnitResultArchiver',
		testResults: 'build/tests/testrunner.xml,build_win/tests/testrunner.xml'])

	archive 'build*/*.tar.gz,build_win*/*.zip'
} // end node


def build(buildDir, cmakeOptions, target) {
	sh "rm -rf ${buildDir} && mkdir ${buildDir}"

	stage 'Configure'
	sh "cd ${buildDir} && cmake ../ogs ${cmakeOptions}"

	stage 'Build'
	sh "cd ${buildDir} && make -j 4 ${target}"
}
