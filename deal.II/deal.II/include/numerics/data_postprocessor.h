//---------------------------------------------------------------------------
//    $Id$
//    Version: $Name$
//
//    Copyright (C) 2007 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//---------------------------------------------------------------------------
#ifndef __deal2__data_postprocessor_h
#define __deal2__data_postprocessor_h



#include <base/subscriptor.h>
#include <base/tensor.h>
#include <lac/vector.h>
#include <fe/fe_update_flags.h>

#include <vector>
#include <string>

DEAL_II_NAMESPACE_OPEN


/*
 * For the (graphical) output of a FE solution one frequently wants to include
 * derived quantities, which are calculated from the values of the solution and
 * possibly the first and second derivates of the solution. This class offers
 * the interface to perform such a postprocessing. Given the values and
 * derivatives of provided dataon given points of a cell, new quantities can be
 * calculated.
 *
 * A data vector and an object of a derived class can be given to the
 * <tt>DataOut::add_data_vector</tt> function, which will write the derived
 * quantities instead of the provided data to the output file. Note, that the
 * DataPostprocessor has to live until @p build_patches has been
 * called. DataOutFaces and DataOutRotation can be used as well.
 *
 * In order not to perform needless calculations, DataPostprocessor has to
 * provide the information, which input data is needed for the calculation of
 * the derived quantities, i.e. whether it needs the values, the first
 * derivative and/or the second derivative of the provided
 * data. DataPostprocessor objects which are used in combination with a
 * DataOutFaces object can also ask for the normal vectors at each point. The
 * information, which data is needed has to be provided via the UpdateFlags
 * returned by the virtual function @p get_needed_update_flags. It is your
 * responsibility to use only those values which were updated in the calculation
 * of derived quantities. The DataOut object will provide references to the
 * requested data in the call to @p compute_derived_quantities.
 *
 * Furthermore, derived classes have to implement the @p get_names and @p
 * n_output_variables functions, where the number of output variables returned
 * by the latter function has to match the size of the vector returned by the
 * former. Furthermore, this number has to match the number of computed
 * quantities, of course.
 *
 * @ingroup output
 * @author Tobias Leicht, 2007
 */
template <int dim>
class DataPostprocessor: public Subscriptor
{
  public:
				     /**
				      * Virtual desctructor for safety. Does not
				      * do anything so far.
				      */
    virtual ~DataPostprocessor ();

				     /**
				      * This is the main function which actually
				      * performs the postprocessing. The first
				      * argument is a reference to the
				      * postprocessed data which has correct
				      * size already and must be filled by this
				      * function. @p uh is a reference to a
				      * vector of data values at all points, @p
				      * duh the same for gradients, @p dduh for
				      * second derivatives and @p normals is a
				      * reference to the normal vectors. Note,
				      * that the last four references will only
				      * contain valid data, if the respective
				      * flags are returned by @p
				      * get_needed_update_flags, otherwise those
				      * vectors will be in an unspecified
				      * state. @p normals will always be an
				      * empty vector when working on cells, not
				      * on faces.
				      *
				      * This function is called, when the
				      * original data vector is scalar valued.
				      */
    virtual void compute_derived_quantities (std::vector<Vector<double> >      &computed_quantities,
					     const std::vector<double>         &uh,
					     const std::vector<Tensor<1,dim> > &duh,
					     const std::vector<Tensor<2,dim> > &dduh,
					     const std::vector<Point<dim> >    &normals) const;
				     /**
				      * Same as above function, but this one is
				      * called in case of vector-valued original
				      * data.
				      */
    virtual void compute_derived_quantities (std::vector<Vector<double> >                    &computed_quantities,
					     const std::vector<Vector<double> >              &uh,
					     const std::vector<std::vector<Tensor<1,dim> > > &duh,
					     const std::vector<std::vector<Tensor<2,dim> > > &dduh,
					     const std::vector<Point<dim> >                  &normals) const;

				     /**
				      * Return the vector of strings describing
				      * the names of the computed quantities.
				      */
    virtual std::vector<std::string> get_names () const=0;

				     /**
				      * Return, which data has to be provided to
				      * compute the derived quantities. This has
				      * to be a combination of @p update_values,
				      * @p update_gradients and @p
				      * update_second_derivatives. If the
				      * DataPostprocessor is to be used in
				      * combination with DataOutFaces, you may
				      * also ask for a update of normals via the
				      * @p update_normal_vectors flag.
				      */
    virtual UpdateFlags get_needed_update_flags () const=0;

				     /**
				      * Number of postprocessed variables. Has
				      * to match the number of entries filled by
				      * @p compute_derived_quantities as well as
				      * the size of the vector of names returned
				      * by @p get_names.
				      */
    virtual unsigned int n_output_variables() const=0;
    
};

DEAL_II_NAMESPACE_CLOSE

#endif
