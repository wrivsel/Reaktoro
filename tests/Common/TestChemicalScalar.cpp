// Reaktor is a C++ library for computational reaction modelling.
//
// Copyright (C) 2014 Allan Leal
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

#include "TestChemicalScalar.hpp"

// Reaktor includes
#include <Reaktor/Reaktor.hpp>

namespace Reaktor {
namespace {

auto createChemicalScalar() -> ChemicalScalar
{
    const double val = 1.0;
    const double ddt = 2.0;
    const double ddp = 3.0;
    const Vector ddn = {1.0, 2.0, 3.0};
    ChemicalScalar scalar(val, ddt, ddp, ddn);
    return scalar;
}

auto createChemicalVector() -> ChemicalVector
{
    Vector val = {1.0, 2.0};
    Vector ddt = {2.0, 3.0};
    Vector ddp = {4.0, 5.0};
    Matrix ddn = {1.0, 3.0, 2.0, 4.0};
    ddn.reshape(2, 2);
    ChemicalVector vec(val, ddt, ddp, ddn);
    return vec;
}

#define ASSERT_EQUAL_ARMA(expected, actual) ASSERT(arma::norm(expected - actual)/arma::norm(actual) < 1e-15)

auto test_ChemicalScalar_val() -> void
{
    ChemicalScalar scalar = createChemicalScalar();
    ASSERT_EQUAL(1.0, scalar.val());
}

auto test_ChemicalScalar_ddt() -> void
{
    ChemicalScalar scalar = createChemicalScalar();
    ASSERT_EQUAL(2.0, scalar.ddt());
}

auto test_ChemicalScalar_ddp() -> void
{
    ChemicalScalar scalar = createChemicalScalar();
    ASSERT_EQUAL(3.0, scalar.ddp());
}

auto test_ChemicalScalar_ddn() -> void
{
    ChemicalScalar scalar = createChemicalScalar();
    Vector expected = {1.0, 2.0, 3.0};
    ASSERT_EQUAL_ARMA(expected, scalar.ddn());
}

auto test_ChemicalScalar_assignOperatorChemicalVectorRow() -> void
{
    ChemicalVector vec = createChemicalVector();
    ChemicalVectorRow row(vec, 1);
    ChemicalScalar scalar = row;
    ASSERT_EQUAL(2.0, scalar.val());
    ASSERT_EQUAL(3.0, scalar.ddt());
    ASSERT_EQUAL(5.0, scalar.ddp());
    ASSERT_EQUAL(3.0, scalar.ddn()[0]);
    ASSERT_EQUAL(4.0, scalar.ddn()[1]);
}

auto test_ChemicalScalar_assignOperatorChemicalVectorConstRow() -> void
{
    const ChemicalVector vec = createChemicalVector();
    ChemicalVectorConstRow row(vec, 1);
    ChemicalScalar scalar = row;
    ASSERT_EQUAL(2.0, scalar.val());
    ASSERT_EQUAL(3.0, scalar.ddt());
    ASSERT_EQUAL(5.0, scalar.ddp());
    ASSERT_EQUAL(3.0, scalar.ddn()[0]);
    ASSERT_EQUAL(4.0, scalar.ddn()[1]);
}

} // namespace

auto testSuiteChemicalScalar() -> cute::suite
{
    cute::suite s;

    s += CUTE(test_ChemicalScalar_val);
    s += CUTE(test_ChemicalScalar_ddt);
    s += CUTE(test_ChemicalScalar_ddp);
    s += CUTE(test_ChemicalScalar_ddn);
    s += CUTE(test_ChemicalScalar_assignOperatorChemicalVectorRow);
    s += CUTE(test_ChemicalScalar_assignOperatorChemicalVectorConstRow);

    return s;
}

} // namespace Reaktor