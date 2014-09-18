// Filename: ConstraintIBKinematics.h
// Created by Amneet Bhalla on 12/10/2011.

// Copyright (c) 2002-2014, Boyce Griffith
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
//    * Neither the name of New York University nor the names of its
//      contributors may be used to endorse or promote products derived from
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

 
#ifndef included_ConstraintIBKinematics
#define included_ConstraintIBKinematics

///////////////////////////////////////// INCLUDES //////////////////////////////////////////


#include <vector>
#include "tbox/Array.h"
#include "tbox/DescribedClass.h"
#include "tbox/Serializable.h"
#include "tbox/Database.h"
#include "ibtk/LDataManager.h"
#include "ibtk/LData.h"

//C++ INCLUDES


namespace IBAMR
{
  
/*!
 * \brief Class ConstraintIBKinematics encapsulates structure information and provides abstraction to get 
 * kinematics (deformational or imposed) of immersed structure to ConstraintIBMethod class.
 */

class ConstraintIBKinematics
    : public virtual SAMRAI::tbox::DescribedClass,
      public SAMRAI::tbox::Serializable
{

  
public:
  
class StructureParameters
{
    
public:  
  
    /*!
     * \brief Constructor.
     */
    StructureParameters(
        SAMRAI::tbox::Pointer<SAMRAI::tbox::Database> input_db,
	IBTK::LDataManager* l_data_manager);
    
    /*!
     * \brief Lagrangian point to tag on this structure.
     */
    inline int 
    getTaggedPtIdx() const
    {
        return d_tagged_pt_idx;
	
    } //getTaggedPtIdx
    
    /*!
     * \brief Get the unlocked components of translational momentum.
     */
    inline SAMRAI::tbox::Array<int>
    getCalculateTranslationalMomentum() const
    {

        return d_calculate_trans_mom;

    } //getCalculateTranslationalMomentum 
    
    /*!
     * \brief Get the unlocked components of rotational momentum.
     */
    inline SAMRAI::tbox::Array<int>
    getCalculateRotationalMomentum() const
    {

        return d_calculate_rot_mom;

    } //getCalculateRotationalMomentum
    

    /*!
     * \brief Check if the structure has translational degree unlocked.
     */
    inline bool
    getStructureIsSelfTranslating() const
    {
        return d_struct_is_self_translating;

    } //getStructureIsSelfTranslating
    
    /*!
     * \brief Check if the structure has rotational degree unlocked.
     */
    inline bool
    getStructureIsSelfRotating() const
    {
        return d_struct_is_self_rotating;

    } //getStructureIsSelfRotating

    /*!
     * \brief The coarsest level on which the structure resides.
     */
    inline int
    getCoarsestLevelNumber() const
    {
        return d_coarsest_ln;

    } //getCoarsestLevelNumber
    
    /*!
     * \brief The finest level on which the structure resides.
     */
    inline int
    getFinestLevelNumber() const
    {
        return d_finest_ln;

    } //getFinestLevelNumber
    
    /*!
     * \brief Global Lagrangian indices managed for this structure.
     */
    inline const std::vector<std::pair<int,int> >& 
    getLagIdxRange() const
    {
        return d_idx_range;
 
    } //getLagIdxRange

    /*!
     * \brief Total number of Lagrangian nodes managed for this structure.
     */
    inline int 
    getTotalNodes() const
    {
        return d_total_nodes;

    } //getTotalNodes

    /*!
     * \brief Lagrangian nodes update method for this structure.
     */
    inline std::string
    getPositionUpdateMethod() const
    {

        return d_lag_position_update_method; 

    } //getPositionUpdateMethod
   
private:
  
     std::string d_lag_position_update_method; 
     int d_coarsest_ln, d_finest_ln;
     std::vector<std::pair<int,int> > d_idx_range;
     int d_total_nodes;
     int d_tagged_pt_idx;
     SAMRAI::tbox::Array<int> d_calculate_trans_mom, d_calculate_rot_mom;
     bool d_struct_is_self_translating, d_struct_is_self_rotating;
    
};
    /*!
     * \brief Constructor.
     */
    ConstraintIBKinematics(
        const std::string& object_name,
        SAMRAI::tbox::Pointer<SAMRAI::tbox::Database> input_db,
	IBTK::LDataManager* l_data_manager,
        bool register_for_restart = true);
    
    /*!
     * \brief Destructor.
     */
    virtual
    ~ConstraintIBKinematics();
    
    /*!
     * \brief Get the object enclosing this structure's parameters.
     */
    inline const StructureParameters&
    getStructureParameters() const
    {
        return d_struct_param;
    }

    /*!
     * \brief Set the kinematics velocity(deformational or imposed) at Lagrangian points managed by this object.
     * 
     * \param Time new time at which kinematics velocity is to be set.
     * 
     * \param incremented_angle_from_reference_axis angle made with x,y & z axis due to rigid rotational velocity.
     * \f$ \theta_n = \theta_{n-1} + \omega_{n-1} \triangle t \f$
     * 
     * \param center_of_mass COM of the structure at current time.
     * 
     * \param tagged_pt_position Coordinates of the tagged point on this structure.
     */
    virtual void
    setNewKinematicsVelocity(
        const double Time,
        const std::vector<double>& incremented_angle_from_reference_axis,
        const std::vector<double>& center_of_mass,
        const std::vector<double>& tagged_pt_position) = 0;

    /*!
     * \brief Get the kinematics velocity at new time for the structure on the specified level.
     * 
     * \param level kinematics velocity of the structure on this level
     */
    virtual const std::vector<std::vector<double> >&
    getNewKinematicsVelocity(const int level) const = 0;
    
    
    /*!
     * \brief Get the kinematics velocity at current time for the structure on the specified level.
     * 
     * \param level kinematics velocity of the structure on this level
     */
    virtual const std::vector<std::vector<double> >&
    getCurrentKinematicsVelocity(const int level) const = 0;
    
    /*!
     * \brief Set the shape of structure at new time for the structure on all levels.
     */
    virtual void
    setNewShape() = 0;

    /*!
     * \brief Get the shape of structure at new time  on the specified level.
     * 
     * \param level new shape of the structure on this level
     */
    virtual const std::vector<std::vector<double> >&
    getNewShape(const int level) const = 0;
    
    /*!
     * \brief Write out object state to the given database.
     *
     * \note An empty default implementation is provided.
     */
    virtual void
    putToDatabase(
        SAMRAI::tbox::Pointer<SAMRAI::tbox::Database> db);
    
///////////////////////////////////////PRIVATE////////////////////////////////// 
private:    
  
    /*!
     * \brief Deleted default ctor.
     */
    ConstraintIBKinematics();
    
    /*!
     * \brief Deleted default copy ctor.
     */
    ConstraintIBKinematics(
        const ConstraintIBKinematics& from);
    
    /*!
     * \brief Deleted default assignment.
     */
    ConstraintIBKinematics&
    operator = (const ConstraintIBKinematics& that);
   
    /*!
     * \brief Object enclosing all the parameters of the structure.
     */ 
    StructureParameters d_struct_param;
    
///////////////////////////////////////PROTECTED////////////////////////////////// 
protected:
  
    /*!
     * Name of the object.
     */
    std::string d_object_name;
    
    /*!
     * If the object is registred for restart.
     */
    bool d_registered_for_restart;
    
       
};// ConstraintIBKinematics


} //IBAMR


/////////////////////////////////// INLINE ///////////////////////////////////////

//#include "ConstraintIBKinematics.I"

/////////////////////////////////////////////////////////////////////////////////

#endif //#ifndef included_ConstraintIBKinematics