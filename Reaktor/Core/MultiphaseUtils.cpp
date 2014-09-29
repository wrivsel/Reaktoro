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

#include "MultiphaseUtils.hpp"

// C++ includes
#include <set>

// Reaktor includes
#include <Reaktor/Common/ThermoVector.hpp>
#include <Reaktor/Common/ChemicalScalar.hpp>
#include <Reaktor/Common/ChemicalVector.hpp>
#include <Reaktor/Core/Multiphase.hpp>
#include <Reaktor/Core/Phase.hpp>
#include <Reaktor/Core/PhaseUtils.hpp>
#include <Reaktor/Core/Species.hpp>
#include <Reaktor/Core/SpeciesUtils.hpp>

namespace Reaktor {

auto numElements(const Multiphase& multiphase) -> unsigned
{
    return multiphase.elements().size();
}

auto numSpecies(const Multiphase& multiphase) -> unsigned
{
    return multiphase.species().size();
}

auto numPhases(const Multiphase& multiphase) -> unsigned
{
    return multiphase.phases().size();
}

auto containsElement(const Multiphase& multiphase, const std::string& element) -> bool
{
    return elementIndex(multiphase, element) < numElements(multiphase);
}

auto containsSpecies(const Multiphase& multiphase, const std::string& species) -> bool
{
    return speciesIndex(multiphase, species) < numSpecies(multiphase);
}

auto containsPhase(const Multiphase& multiphase, const std::string& phase) -> bool
{
    return phaseIndex(multiphase, phase) < numPhases(multiphase);
}

auto elementIndex(const Multiphase& multiphase, const std::string& element) -> Index
{
    const auto& begin = multiphase.elements().begin();
    const auto& end = multiphase.elements().end();
    return std::find(begin, end, element) - begin;
}

auto elementIndices(const Multiphase& multiphase, const std::vector<std::string>& names) -> Indices
{
    Indices indices;
    indices.reserve(names.size());
    for(const std::string& name : names)
        indices.push_back(elementIndex(multiphase, name));
    return indices;
}

auto elementIndicesInSpecies(const Multiphase& multiphase, const Index& ispecies) -> Indices
{
    const Species& species = multiphase.species()[ispecies];
    return elementIndices(multiphase, species.elements());
}

auto elementIndicesInSpecies(const Multiphase& multiphase, const Indices& ispecies) -> Indices
{
    std::set<Index> ielements;
    for(const Index& i : ispecies)
    {
        Indices tmp = elementIndicesInSpecies(multiphase, i);
        ielements.insert(tmp.begin(), tmp.end());
    }
    return Indices(ielements.begin(), ielements.end());
}

auto speciesIndex(const Multiphase& multiphase, const std::string& name) -> Index
{
    const auto& begin = multiphase.species().begin();
    const auto& end = multiphase.species().end();
    const auto sameName = [&](const Species& species) { return species.name() == name; };
    return std::find_if(begin, end, sameName) - begin;
}

auto speciesIndices(const Multiphase& multiphase, const std::vector<std::string>& names) -> Indices
{
    Indices indices;
    indices.reserve(names.size());
    for(const std::string& name : names)
        indices.push_back(speciesIndex(multiphase, name));
    return indices;
}

auto speciesBeginIndexInPhase(const Multiphase& multiphase, const Index& iphase) -> Index
{
    if(iphase < numPhases(multiphase))
    {
        unsigned offset = 0;
        for(unsigned i = 0; i < iphase; ++i)
            offset += numSpecies(multiphase.phases()[i]);
        return offset;
    }
    else return numSpecies(multiphase);
}

auto speciesEndIndexInPhase(const Multiphase& multiphase, const Index& iphase) -> Index
{
    if(iphase < numPhases(multiphase))
        return speciesBeginIndexInPhase(multiphase, iphase) + numSpecies(multiphase.phases()[iphase]);
    else return numSpecies(multiphase);
}

auto speciesIndicesInPhase(const Multiphase& multiphase, const Index& iphase) -> Indices
{
    if(iphase < numPhases(multiphase))
    {
        const unsigned num_species = numSpecies(multiphase.phases()[iphase]);
        Indices indices(num_species);
        const Index first = speciesBeginIndexInPhase(multiphase, iphase);
        std::iota(indices.begin(), indices.end(), first);
        return indices;
    }
    else return Indices();
}

auto speciesIndicesWithElement(const Multiphase& multiphase, const Index& ielement) -> Indices
{
    if(ielement < numElements(multiphase))
    {
        std::set<Index> indices;
        for(unsigned i = 0; i < numSpecies(multiphase); ++i)
            if(containsElement(multiphase.species()[i], multiphase.elements()[ielement]))
                indices.insert(i);
        return Indices(indices.begin(), indices.end());
    }
    else return Indices();
}

auto speciesLocalIndex(const Multiphase& multiphase, const Index& ispecies) -> Index
{
    const Index iphase = phaseIndexWithSpecies(multiphase, ispecies);
    const Index ifirst = speciesBeginIndexInPhase(multiphase, iphase);
    return ispecies - ifirst;
}

auto phaseIndex(const Multiphase& multiphase, const std::string& name) -> Index
{
    const auto& begin = multiphase.phases().begin();
    const auto& end = multiphase.phases().end();
    const auto sameName = [&](const Phase& phase) { return phase.name() == name; };
    return std::find_if(begin, end, sameName) - begin;
}

auto phaseIndices(const Multiphase& multiphase, const std::vector<std::string>& phases) -> Indices
{
    Indices indices;
    indices.reserve(phases.size());
    for(const std::string& name : phases)
        indices.push_back(phaseIndex(multiphase, name));
    return indices;
}

auto phaseIndexWithSpecies(const Multiphase& multiphase, const Index& ispecies) -> Index
{
    if(ispecies < numSpecies(multiphase))
    {
        for(unsigned iphase = 0; iphase < numPhases(multiphase); ++iphase)
            if(ispecies < speciesEndIndexInPhase(multiphase, iphase))
                return iphase;
    }
    return numPhases(multiphase);
}

auto phaseIndicesWithSpecies(const Multiphase& multiphase, const Indices& ispecies) -> Indices
{
	std::set<Index> iphases;
	for(const Index& i : ispecies)
		iphases.insert(phaseIndexWithSpecies(multiphase, i));
	return Indices(iphases.begin(), iphases.end());
}

auto indexMapSpeciesToElements(const Multiphase& multiphase) -> std::vector<Indices>
{
	const unsigned num_species = numSpecies(multiphase);
	std::vector<Indices> map(num_species);
	for(unsigned i = 0; i < num_species; ++i)
		map[i] = elementIndicesInSpecies(multiphase, i);
	return map;
}

auto indexMapElementToSpecies(const Multiphase& multiphase) -> std::vector<Indices>
{
	const unsigned num_elements = numElements(multiphase);
	std::vector<Indices> map(num_elements);
	for(unsigned i = 0; i < num_elements; ++i)
		map[i] = speciesIndicesWithElement(multiphase, i);
	return map;
}

auto indexMapPhaseToSpecies(const Multiphase& multiphase) -> std::vector<Indices>
{
	const unsigned num_phases = numPhases(multiphase);
	std::vector<Indices> map(num_phases);
	for(unsigned i = 0; i < num_phases; ++i)
		map[i] = speciesIndicesInPhase(multiphase, i);
	return map;
}

auto indexMapSpeciesToPhase(const Multiphase& multiphase) -> Indices
{
	const unsigned num_species = numSpecies(multiphase);
	Indices map(num_species);
	for(unsigned i = 0; i < num_species; ++i)
		map[i] = phaseIndexWithSpecies(multiphase, i);
	return map;
}

auto formulaMatrix(const Multiphase& multiphase) -> Matrix
{
    const auto& elements = multiphase.elements();
    const auto& species = multiphase.species();
    const auto& numElements = elements.size();
    const auto& numSpecies = species.size();
    Matrix res(numElements, numSpecies);
    for(unsigned i = 0; i < numElements; ++i)
        for(unsigned j = 0; j < numSpecies; ++j)
            res(i, j) = elementAtoms(species[j], elements[i]);
    return res;
}

auto block(const Multiphase& multiphase, const Index& iphase, const Vector& vec) -> VectorView
{
    const Index ibegin = speciesBeginIndexInPhase(multiphase, iphase);
    const Index iend = speciesEndIndexInPhase(multiphase, iphase);
    return vec.subvec(ibegin, iend-1);
}

auto block(const Multiphase& multiphase, const Index& iphase, const Matrix& mat) -> MatrixView
{
    const Index ibegin = speciesBeginIndexInPhase(multiphase, iphase);
    const Index iend = speciesEndIndexInPhase(multiphase, iphase);
    return mat.submat(ibegin, ibegin, iend-1, iend-1);
}

auto volumes(const Multiphase& multiphase, double T, double P) -> ThermoVector
{
    return volumes(multiphase.species(), T, P);
}

auto entropies(const Multiphase& multiphase, double T, double P) -> ThermoVector
{
    return entropies(multiphase.species(), T, P);
}

auto helmholtzEnergies(const Multiphase& multiphase, double T, double P) -> ThermoVector
{
    return helmholtzEnergies(multiphase.species(), T, P);
}

auto internalEnergies(const Multiphase& multiphase, double T, double P) -> ThermoVector
{
    return internalEnergies(multiphase.species(), T, P);
}

auto enthalpies(const Multiphase& multiphase, double T, double P) -> ThermoVector
{
    return enthalpies(multiphase.species(), T, P);
}

auto gibbsEnergies(const Multiphase& multiphase, double T, double P) -> ThermoVector
{
    return gibbsEnergies(multiphase.species(), T, P);
}

auto heatCapacitiesCp(const Multiphase& multiphase, double T, double P) -> ThermoVector
{
    return heatCapacitiesCp(multiphase.species(), T, P);
}

template<typename Function>
auto speciesProperties(const Multiphase& multiphase, const Vector& n, Function func) -> ChemicalVector
{
    const unsigned nspecies = numSpecies(multiphase);
    Vector res_val(nspecies);
    Vector res_ddt(nspecies);
    Vector res_ddp(nspecies);
    Matrix res_ddn = zeros(nspecies, nspecies);
    Index ifirst = 0;
    for(const Phase& phase : multiphase.phases())
    {
        const Index nspecies_phase = numSpecies(phase);
        const Index ilast = ifirst + nspecies_phase - 1;
        const Vector n_phase = n.subvec(ifirst, ilast);
        const ChemicalVector res_phase = func(phase, n_phase);
        res_val.subvec(ifirst, ilast) = res_phase.val();
        res_ddt.subvec(ifirst, ilast) = res_phase.ddt();
        res_ddp.subvec(ifirst, ilast) = res_phase.ddp();
        res_ddn.submat(ifirst, ifirst, ilast, ilast) = res_phase.ddn();
        ifirst += nspecies_phase;
    }
    return {res_val, res_ddt, res_ddp, res_ddn};
}

template<typename Function>
auto phasesProperties(const Multiphase& multiphase, const Vector& n, Function func) -> ChemicalVector
{
    const unsigned nphases = numPhases(multiphase);
    const unsigned nspecies = numSpecies(multiphase);
    Vector res_val(nphases);
    Vector res_ddt(nphases);
    Vector res_ddp(nphases);
    Matrix res_ddn = zeros(nphases, nspecies);
    Index ifirst = 0;
    for(unsigned i = 0; i < nphases; ++i)
    {
        const Phase& phase = multiphase.phases()[i];
        const Index nspecies_phase = numSpecies(phase);
        const Index ilast = ifirst + nspecies_phase;
        const Vector n_phase = n.subvec(ifirst, ilast);
        const ChemicalScalar res_phase = func(phase, n_phase);
        res_val[i] = res_phase.val();
        res_ddt[i] = res_phase.ddt();
        res_ddp[i] = res_phase.ddp();
        res_ddn.row(i).subvec(ifirst, ilast) = res_phase.ddn();
        ifirst += nspecies_phase;
    }
    return {res_val, res_ddt, res_ddp, res_ddn};
}

auto molarFractions(const Multiphase& multiphase, const Vector& n) -> ChemicalVector
{
    auto func = [](const Phase& phase, const Vector& n) { return molarFractions(phase, n); };
    return speciesProperties(multiphase, n, func);
}

auto concentrations(const Multiphase& multiphase, const Vector& n) -> ChemicalVector
{
    auto func = [](const Phase& phase, const Vector& n) { return concentrations(phase, n); };
    return speciesProperties(multiphase, n, func);
}

auto activities(const Multiphase& multiphase, double T, double P, const Vector& n) -> ChemicalVector
{
    auto func = [=](const Phase& phase, const Vector& n) { return activities(phase, T, P, n); };
    return speciesProperties(multiphase, n, func);
}

auto densities(const Multiphase& multiphase, double T, double P, const Vector& n) -> ChemicalVector
{
    auto func = [=](const Phase& phase, const Vector& n) { return density(phase, T, P, n); };
    return phasesProperties(multiphase, n, func);
}

} // namespace Reaktor
