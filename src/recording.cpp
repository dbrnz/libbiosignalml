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

#include <biosignalml/recording.h>

using namespace bsml ;


Interval::Ptr Recording::new_interval(const double start, const double duration,
/*----------------------------------------------------------------------------*/
                                      const std::string &units)
{
  rdf::URI u = timeline() && timeline()->is_valid() ? timeline()->uri().make_URI()
                                                    : uri().make_URI() ;
  auto tm = Interval::create(u, start, duration, units, timeline()) ;
  add_resource<Interval>(tm) ;
  return tm ;
  }

Instant::Ptr Recording::new_instant(const double start, const std::string &units)
/*-----------------------------------------------------------------------------*/
{
  rdf::URI u = timeline() && timeline()->is_valid() ? timeline()->uri().make_URI()
                                                    : uri().make_URI() ;
  auto tm = Instant::create(u, start, units, timeline()) ;
  add_resource<Instant>(tm) ;
  return tm ;
  }


Event::Ptr Recording::get_event(const rdf::URI &uri)
/*------------------------------------------------*/
{
  return get_resource<Event>(uri) ;
  }

Event::Ptr Recording::get_event(const std::string &uri)
/*---------------------------------------------------*/
{
  return get_event(rdf::URI(uri)) ;
  }

std::list<rdf::URI> Recording::get_event_uris(const rdf::URI &type)
/*---------------------------------------------------------------*/
{
  return get_resource_uris<Event>() ;  // lambda returning bool
  }


Annotation::Ptr Recording::get_annotation(const rdf::URI &uri)
/*----------------------------------------------------------*/
{
  return get_resource<Annotation>(uri) ;
  }

Annotation::Ptr Recording::get_annotation(const std::string &uri)
/*-------------------------------------------------------------*/
{
  return get_annotation(rdf::URI(uri)) ;
  }

std::list<rdf::URI> Recording::get_annotation_uris(void)
/*----------------------------------------------------*/
{
  auto result = std::list<rdf::URI>{} ;
  auto stmnt = m_graph->get_statements(rdf::Node(), rdf::RDF::type, bsml::BSML::Annotation) ;
  if (!stmnt.end()) {
    do {
      auto ann_node = stmnt.get_subject() ;
      if (!m_graph->contains(rdf::Node(), rdf::PRV::precededBy, ann_node)) {
        auto subj = m_graph->get_statements(ann_node, rdf::DCT::subject, rdf::Node()) ;
        if (!subj.end()) {
          auto subject = subj.get_object() ;
          if (subject == uri()
           || (m_graph->contains(subject, rdf::RDF::type, bsml::BSML::Segment)
            && m_graph->contains(subject, rdf::DCT::source, uri()))
           || (m_graph->contains(subject, rdf::RDF::type, bsml::BSML::Signal)
            && m_graph->contains(subject, bsml::BSML::recording, uri()))
              ) result.push_back(rdf::URI(ann_node)) ;
          }
        }
      } while (!stmnt.next()) ;
    }
  return result ;
  }
