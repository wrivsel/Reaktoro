// Reaktoro is a unified framework for modeling chemically reactive systems.
//
// Copyright (C) 2014-2015 Allan Leal
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

// Boost includes
#include <boost/python.hpp>
namespace py = boost::python;

// Reaktoro includes
#include <Reaktoro/Common/Index.hpp>
#include <Reaktoro/Core/ChemicalState.hpp>
#include <Reaktoro/Core/ChemicalSystem.hpp>
#include <Reaktoro/Core/Partition.hpp>
#include <Reaktoro/Core/ReactionSystem.hpp>
#include <Reaktoro/Utils/ChemicalSolver.hpp>

namespace Reaktoro {

auto export_ChemicalSolver() -> void
{
    auto setState1 = static_cast<void(ChemicalSolver::*)(const ChemicalState&)>(&ChemicalSolver::setState);
    auto setState2 = static_cast<void(ChemicalSolver::*)(const ChemicalState&, const Indices&)>(&ChemicalSolver::setState);

    py::class_<ChemicalSolver>("ChemicalSolver")
        .def(py::init<>())
        .def(py::init<const ChemicalSystem&, Index>())
        .def(py::init<const ReactionSystem&, Index>())
        .def("setPartition", &ChemicalSolver::setPartition)
        .def("setState", setState1)
        .def("setState", setState2)
        .def("state", &ChemicalSolver::state, py::return_internal_reference<>())
        .def("states", &ChemicalSolver::states, py::return_internal_reference<>())
        .def("porosity", &ChemicalSolver::porosity)
        .def("saturations", &ChemicalSolver::saturations)
        .def("densities", &ChemicalSolver::densities)
        .def("equilibrate", &ChemicalSolver::equilibrate)
        .def("react", &ChemicalSolver::react)
        ;
}

} // namespace Reaktoro