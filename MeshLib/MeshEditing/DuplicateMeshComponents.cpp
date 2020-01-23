/**
 * \file
 * \author Karsten Rink
 * \date   2014-03-25
 * \brief  Implementation of Duplicate functions.
 *
 * \copyright
 * Copyright (c) 2012-2020, OpenGeoSys Community (http://www.opengeosys.org)
 *            Distributed under a Modified BSD License.
 *              See accompanying file LICENSE.txt or
 *              http://www.opengeosys.org/project/license
 *
 */

#include "DuplicateMeshComponents.h"

#include "MeshLib/Mesh.h"
#include "MeshLib/Elements/Elements.h"

namespace MeshLib
{
std::vector<MeshLib::Node*> copyNodeVector(
    const std::vector<MeshLib::Node*>& nodes)
{
    const std::size_t nNodes(nodes.size());
    std::vector<MeshLib::Node*> new_nodes;
    new_nodes.reserve(nNodes);
    for (std::size_t k = 0; k < nNodes; ++k)
    {
        new_nodes.push_back(
            new MeshLib::Node(nodes[k]->getCoords(), new_nodes.size()));
    }
    return new_nodes;
}

std::vector<MeshLib::Element*> copyElementVector(
    const std::vector<MeshLib::Element*>& elements,
    const std::vector<MeshLib::Node*>& nodes)
{
    const std::size_t nElements(elements.size());
    std::vector<MeshLib::Element*> new_elements;
    new_elements.reserve(nElements);
    for (std::size_t k = 0; k < nElements; ++k)
    {
        new_elements.push_back(copyElement(elements[k], nodes));
    }
    return new_elements;
}

/// Copies an element without change, using the nodes vector from the result
/// mesh.
template <typename E>
MeshLib::Element* copyElement(MeshLib::Element const* const element,
                              const std::vector<MeshLib::Node*>& nodes,
                              std::vector<std::size_t> const* const id_map)
{
    unsigned const number_of_element_nodes(element->getNumberOfNodes());
    auto** new_nodes = new MeshLib::Node*[number_of_element_nodes];
    if (id_map)
    {
        for (unsigned i = 0; i < number_of_element_nodes; ++i)
        {
            new_nodes[i] = nodes[(*id_map)[element->getNode(i)->getID()]];
        }
    }
    else
    {
        for (unsigned i = 0; i < number_of_element_nodes; ++i)
        {
            new_nodes[i] = nodes[element->getNode(i)->getID()];
        }
    }
    return new E(new_nodes);
}

MeshLib::Element* copyElement(
    MeshLib::Element const* const element,
    const std::vector<MeshLib::Node*>& nodes,
    std::vector<std::size_t> const* const id_map)
{
    switch (element->getGeomType())
    {
        case MeshElemType::LINE:
            return copyElement<MeshLib::Line>(element, nodes, id_map);
        case MeshElemType::TRIANGLE:
            return copyElement<MeshLib::Tri>(element, nodes, id_map);
        case MeshElemType::QUAD:
            return copyElement<MeshLib::Quad>(element, nodes, id_map);
        case MeshElemType::TETRAHEDRON:
            return copyElement<MeshLib::Tet>(element, nodes, id_map);
        case MeshElemType::HEXAHEDRON:
            return copyElement<MeshLib::Hex>(element, nodes, id_map);
        case MeshElemType::PYRAMID:
            return copyElement<MeshLib::Pyramid>(element, nodes, id_map);
        case MeshElemType::PRISM:
            return copyElement<MeshLib::Prism>(element, nodes, id_map);
        default:
        {
    ERR ("Error: Unknown element type.");
            return nullptr;
        }
    }
}

std::vector<MeshLib::Element*> cloneElements(
    std::vector<MeshLib::Element*> const& elements)
{
    std::vector<MeshLib::Element*> cloned_elements;
    cloned_elements.reserve(elements.size());
    std::transform(begin(elements), end(elements),
                   std::back_inserter(cloned_elements),
                   [](MeshLib::Element* const e) { return e->clone(); });
    return cloned_elements;
}

}  // namespace MeshLib
