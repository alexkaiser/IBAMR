// Filename: AdvDiffStochasticForcing.h
// Created on 29 Apr 2011 by Boyce Griffith
//
// Copyright (c) 2002-2017, Boyce Griffith
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of The University of North Carolina nor the names of
//      its contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef included_IBAMR_AdvDiffStochasticForcing
#define included_IBAMR_AdvDiffStochasticForcing

/////////////////////////////// INCLUDES /////////////////////////////////////

#include <string>
#include <vector>

#include "CellVariable.h"
#include "IntVector.h"
#include "PatchLevel.h"
#include "SideVariable.h"
#include "VariableContext.h"
#include "ibtk/CartGridFunction.h"
#include "ibtk/ibtk_utilities.h"
#include "muParser.h"
#include "tbox/Array.h"
#include "tbox/Pointer.h"

namespace IBAMR
{
class AdvDiffSemiImplicitHierarchyIntegrator;
} // namespace IBAMR
namespace SAMRAI
{
namespace hier
{
template <int DIM>
class Patch;
template <int DIM>
class PatchHierarchy;
template <int DIM>
class Variable;
} // namespace hier
namespace tbox
{
class Database;
} // namespace tbox
} // namespace SAMRAI

/////////////////////////////// CLASS DEFINITION /////////////////////////////

namespace IBAMR
{
/*!
 * \brief Class AdvDiffStochasticForcing provides an interface for specifying a
 * stochastic forcing term for cell-centered advection-diffusion solver solver.
 */
class AdvDiffStochasticForcing : public IBTK::CartGridFunction
{
public:
    /*!
     * \brief This constructor creates Variable and VariableContext objects for
     * storing the stochastic fluxes at the faces of the Cartesian grid.
     */
    AdvDiffStochasticForcing(std::string object_name,
                             SAMRAI::tbox::Pointer<SAMRAI::tbox::Database> input_db,
                             SAMRAI::tbox::Pointer<SAMRAI::pdat::CellVariable<NDIM, double> > C_var,
                             const AdvDiffSemiImplicitHierarchyIntegrator* adv_diff_solver);

    /*!
     * \brief Empty destructor.
     */
    ~AdvDiffStochasticForcing() = default;

    /*!
     * \name Methods to set patch data.
     */
    //\{

    /*!
     * \brief Indicates whether the concrete AdvDiffStochasticForcing object is
     * time-dependent.
     */
    bool isTimeDependent() const override;

    /*!
     * \brief Evaluate the function on the patch interiors on the specified
     * levels of the patch hierarchy.
     */
    void setDataOnPatchHierarchy(const int data_idx,
                                 SAMRAI::tbox::Pointer<SAMRAI::hier::Variable<NDIM> > var,
                                 SAMRAI::tbox::Pointer<SAMRAI::hier::PatchHierarchy<NDIM> > hierarchy,
                                 const double data_time,
                                 const bool initial_time = false,
                                 const int coarsest_ln = -1,
                                 const int finest_ln = -1) override;

    /*!
     * \brief Evaluate the function on the patch interior.
     */
    void setDataOnPatch(const int data_idx,
                        SAMRAI::tbox::Pointer<SAMRAI::hier::Variable<NDIM> > var,
                        SAMRAI::tbox::Pointer<SAMRAI::hier::Patch<NDIM> > patch,
                        const double data_time,
                        const bool initial_time = false,
                        SAMRAI::tbox::Pointer<SAMRAI::hier::PatchLevel<NDIM> > patch_level =
                            SAMRAI::tbox::Pointer<SAMRAI::hier::PatchLevel<NDIM> >(NULL)) override;

    //\}

protected:
    /*!
     * The object name is used for error/warning reporting.
     */
    std::string d_object_name;

private:
    /*!
     * \brief Default constructor.
     *
     * \note This constructor is not implemented and should not be used.
     */
    AdvDiffStochasticForcing() = delete;

    /*!
     * \brief Copy constructor.
     *
     * \note This constructor is not implemented and should not be used.
     *
     * \param from The value to copy to this object.
     */
    AdvDiffStochasticForcing(const AdvDiffStochasticForcing& from) = delete;

    /*!
     * \brief Assignment operator.
     *
     * \note This operator is not implemented and should not be used.
     *
     * \param that The value to assign to this object.
     *
     * \return A reference to this object.
     */
    AdvDiffStochasticForcing& operator=(const AdvDiffStochasticForcing& that) = delete;

    /*!
     * Pointer to the concentration variable associated with this source term
     * generator.
     */
    SAMRAI::tbox::Pointer<SAMRAI::pdat::CellVariable<NDIM, double> > d_C_var;

    /*!
     * Concentration-dependent flux scaling function.
     */
    mu::Parser d_f_parser;

    /*!
     * Pointer to the advection-diffusion solver object that is using this
     * stochastic source term generator.
     */
    const AdvDiffSemiImplicitHierarchyIntegrator* const d_adv_diff_solver;

    /*!
     * Weighting data.
     */
    double d_std = std::numeric_limits<double>::quiet_NaN();
    int d_num_rand_vals = 0;
    std::vector<SAMRAI::tbox::Array<double> > d_weights;

    /*!
     * Boundary condition scalings.
     */
    double d_dirichlet_bc_scaling = std::sqrt(2.0), d_neumann_bc_scaling = 0.0;

    /*!
     * VariableContext and Variable objects for storing the components of the
     * stochastic fluxes.
     */
    SAMRAI::tbox::Pointer<SAMRAI::hier::VariableContext> d_context;
    SAMRAI::tbox::Pointer<SAMRAI::pdat::CellVariable<NDIM, double> > d_C_cc_var;
    int d_C_current_cc_idx = IBTK::invalid_index, d_C_half_cc_idx = IBTK::invalid_index,
        d_C_new_cc_idx = IBTK::invalid_index;
    SAMRAI::tbox::Pointer<SAMRAI::pdat::SideVariable<NDIM, double> > d_F_sc_var;
    int d_F_sc_idx = IBTK::invalid_index;
    std::vector<int> d_F_sc_idxs;
};
} // namespace IBAMR

//////////////////////////////////////////////////////////////////////////////

#endif //#ifndef included_IBAMR_AdvDiffStochasticForcing
