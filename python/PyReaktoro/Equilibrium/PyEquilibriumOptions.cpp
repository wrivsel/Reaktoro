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

#include "PyEquilibriumOptions.hpp"

// Boost includes
#include <boost/python.hpp>
namespace py = boost::python;

// Reaktoro includes
#include <Reaktoro/Equilibrium/EquilibriumOptions.hpp>

namespace Reaktoro {

auto export_EquilibriumOptions() -> void
{
    py::enum_<GibbsHessian>("GibbsHessian")
        .value("Exact", GibbsHessian::Exact)
        .value("ExactDiagonal", GibbsHessian::ExactDiagonal)
        .value("Approximation", GibbsHessian::Approximation)
        .value("ApproximationDiagonal", GibbsHessian::ApproximationDiagonal)
        ;

    py::class_<EquilibriumOptions>("EquilibriumOptions")
        .def_readwrite("epsilon", &EquilibriumOptions::epsilon)
        .def_readwrite("warmstart", &EquilibriumOptions::warmstart)
        .def_readwrite("hessian", &EquilibriumOptions::hessian)
        .def_readwrite("method", &EquilibriumOptions::method)
        .def_readwrite("optimum", &EquilibriumOptions::optimum)
        .def_readwrite("nonlinear", &EquilibriumOptions::nonlinear)
        ;
}

} // namespace Reaktoro
