/******************************************************************************
 *                                                                            *
 *  BioSignalML Management in C++                                             *
 *                                                                            *
 *  Copyright (c) 2010-2015  David Brooks                                     *
 *                                                                            *
 *  Licensed under the Apache License, Version 2.0 (the "License");           *
 *  you may not use this file except in compliance with the License.          *
 *  You may obtain a copy of the License at                                   *
 *                                                                            *
 *      http://www.apache.org/licenses/LICENSE-2.0                            *
 *                                                                            *
 *  Unless required by applicable law or agreed to in writing, software       *
 *  distributed under the License is distributed on an "AS IS" BASIS,         *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
 *  See the License for the specific language governing permissions and       *
 *  limitations under the License.                                            *
 *                                                                            *
 ******************************************************************************/

#include <biosignalml/data/timeseries.h>

#include <cassert>
#include <algorithm>


using namespace bsml ;

data::TimeSeries::TimeSeries()
/*--------------------------*/
: m_times(std::vector<double>()), m_data(std::vector<double>())
{
  }

data::TimeSeries::TimeSeries(const size_t size)
/*-------------------------------------------*/
: m_times(std::vector<double>(size)), m_data(std::vector<double>(size))
{
  }

data::TimeSeries::TimeSeries(const std::vector<double> &times, const std::vector<double> &data)
/*-------------------------------------------------------------------------------------------*/
: m_times(times), m_data(data)
{
  assert(times.size() == data.size()) ;
  }

data::Point data::TimeSeries::point(const size_t n) const
/*-----------------------------------------------------*/
{
  return data::Point(m_times[n], m_data[n]) ;
  }

double data::TimeSeries::time(const size_t n) const
/*-----------------------------------------------*/
{
  return m_times[n] ;
  }

double data::TimeSeries::duration(void) const
/*-----------------------------------------*/
{
  return (size() > 1) ? (time(size() - 1) - time(0)) : 0.0 ;
  }

ssize_t data::TimeSeries::index(const double t)
/*-------------------------------------------*/
{
  // Get iterator to first item not less than `t`.
  auto lb = std::lower_bound(m_times.begin(), m_times.end(), t) ;
  return (lb == m_times.end()) ? -1 : std::distance(m_times.begin(), lb) ;
  }


data::UniformTimeSeries::UniformTimeSeries()
/*----------------------------------------*/
: TimeSeries()
{
  m_rate = 0.0 ;
  m_start = 0.0 ;
  }

data::UniformTimeSeries::UniformTimeSeries(const double rate, const std::vector<double> &data,
/*------------------------------------------------------------------------------------------*/
                                           const double start)
: data::TimeSeries()
{
  m_data = data ;
  m_rate = rate ;
  m_start = start ;
  }

data::UniformTimeSeries::UniformTimeSeries(const double rate, const size_t size,
/*----------------------------------------------------------------------------*/
                                           const double start)
: TimeSeries()
{
  m_data = std::vector<double>(size) ;
  m_rate = rate ;
  m_start = start ;
  }

data::Point data::UniformTimeSeries::point(const size_t n) const
/*------------------------------------------------------------*/
{
  return data::Point((m_start + (double)n)/m_rate, m_data[n]) ;
  }

double data::UniformTimeSeries::time(const size_t n) const
/*------------------------------------------------------*/
{
  return (m_start + (double)n)/m_rate ;
  }

ssize_t data::UniformTimeSeries::index(const double t)
/*--------------------------------------------------*/
{
  return (t < m_start) ? -1 : (ssize_t)std::floor(m_rate*(t - m_start)) ;
  }
