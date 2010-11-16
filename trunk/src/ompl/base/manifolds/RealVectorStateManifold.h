/*********************************************************************
* Software License Agreement (BSD License)
* 
*  Copyright (c) 2010, Rice University
*  All rights reserved.
* 
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
* 
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Rice University nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
* 
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

/* Author: Ioan Sucan */

#ifndef OMPL_BASE_MANIFOLDS_REAL_VECTOR_STATE_MANIFOLD_
#define OMPL_BASE_MANIFOLDS_REAL_VECTOR_STATE_MANIFOLD_

#include "ompl/base/StateManifold.h"
#include "ompl/base/manifolds/RealVectorBounds.h"
#include <vector>
#include <string>
#include <map>

namespace ompl
{
    namespace base
    {
	
	/** \brief State sampler for the R<sup>n</sup> manifold */
	class RealVectorStateSampler : public ManifoldStateSampler
	{
	public:
	    
	    RealVectorStateSampler(const StateManifold *manifold) : ManifoldStateSampler(manifold)
	    {
	    }
	    
	    virtual void sampleUniform(State *state);	    
	    virtual void sampleUniformNear(State *state, const State *near, const double distance);
	    virtual void sampleGaussian(State *state, const State *mean, const double stdDev);	    
	};
	
	/** \brief A manifold representing R<sup>n</sup>. The distance function is the L2 norm. */
	class RealVectorStateManifold : public StateManifold
	{
	public:

	    /** \brief The definition of a state in R<sup>n</sup> */
	    class StateType : public State
	    {
	    public:
		StateType(void) : State()
		{
		}
		
		/** \brief Access element i of values.  This does not
		    check whether the index is within bounds */
		double operator[](unsigned int i) const
		{
		    return values[i];
		}
		
		/** \brief Access element i of values.  This does not
		    check whether the index is within bounds */
		double& operator[](unsigned int i)
		{
		    return values[i];
		}
		
		/** \brief The value of the actual vector in R<sup>n</sup> */
		double *values;
	    };
	    
	    /** \brief Constructor. The dimension of of the space needs to be specified. A space representing
		R<sup>dim</sup> will be instantiated */
            RealVectorStateManifold(unsigned int dim = 0) : StateManifold(), dimension_(dim), bounds_(dim),  stateBytes_(dim * sizeof(double))
	    {
		name_ = "RealVector" + name_;
		dimensionNames_.resize(dim, "");
	    }
	    
	    virtual ~RealVectorStateManifold(void)
	    {	
	    }
	    
	    /** \brief Increase the dimensionality of the manifold by 1. Optionally, bounds can be specified for this added dimension. */
	    void addDimension(double minBound = 0.0, double maxBound = 0.0);

	    /** \brief Increase the dimensionality of the manifold by 1 and specify the name of this dimension. Optionally, bounds can be specified for this added dimension. This function is a wrapper for the previous definition of addDimension(). */
	    void addDimension(const std::string &name, double minBound = 0.0, double maxBound = 0.0);
	    
	    /** \brief Set the bounds of this manifold. This defines
		the range of the space in which sampling is
		performed. */
	    void setBounds(const RealVectorBounds &bounds);

	    /** \brief Get the bounds for this manifold */
	    const RealVectorBounds& getBounds(void) const
	    {
		return bounds_;
	    }
	    
	    virtual unsigned int getDimension(void) const;
	    
	    /** \brief Each dimension can optionally have a name associated to it. If it does, this function returns that name.
		Return empty string otherwise */
	    const std::string& getDimensionName(unsigned int index) const;

	    /** \brief Get the index of a specific dimension, by name. Return -1 if name is not found */
	    int getDimensionIndex(const std::string &name) const;
	    
	    /** \brief Set the name of a dimension */
	    void setDimensionName(unsigned int index, const std::string &name);
	    
	    virtual double getMaximumExtent(void) const;

	    virtual void enforceBounds(State *state) const;

	    virtual bool satisfiesBounds(const State *state) const;

	    virtual void copyState(State *destination, const State *source) const;

	    virtual double distance(const State *state1, const State *state2) const;

	    virtual bool equalStates(const State *state1, const State *state2) const;

	    virtual void interpolate(const State *from, const State *to, const double t, State *state) const;

	    virtual ManifoldStateSamplerPtr allocStateSampler(void) const;

	    virtual State* allocState(void) const;
	    
	    virtual void freeState(State *state) const;

	    virtual void printState(const State *state, std::ostream &out) const;

	    virtual void printSettings(std::ostream &out) const;

	    virtual void registerProjections(void);

	    virtual void setup(void);
	    
	protected:
	    
	    /** \brief The dimension of the space */
	    unsigned int                        dimension_;
	    
	    /** \brief The bounds of the space (used for sampling) */
	    RealVectorBounds                    bounds_;
	    
	    /** \brief Optional names for individual dimensions */
	    std::vector<std::string>            dimensionNames_;
	    
	    /** \brief Map from names to index values for dimensions */
	    std::map<std::string, unsigned int> dimensionIndex_;

        private:

	    /** \brief The size of a state, in bytes */
	    std::size_t                         stateBytes_;
            
	};
    }
}

#endif
