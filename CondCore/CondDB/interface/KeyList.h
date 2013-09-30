#ifndef CondCore_CondDB_KeyList_h
#define CondCore_CondDB_KeyList_h

#include "CondCore/CondDB/interface/ORAWrapper.h"
//#include "CondCore/CondDB/interface/IOVProxy.h"
#include "CondFormats/Common/interface/BaseKeyed.h"
#include<vector>
#include<string>

/*
 * KeyList represents a set of payloads each identified by a key  and "valid" at given time
 * Usually these payloads are configuration objects loaded in anvance
 * The model used here calls for all payloads to be "stored" in a single IOVSequence each identified by a unique key 
 * (properly hashed to be mapped in 64bits)
 *
 * the keylist is just a vector of the hashes each corresponding to a key
 * the correspondence position in the vector user-friendly name is kept in 
 * a list of all "names" that is defined in advance and kept in a dictionary at IOVSequence level
 
 *
 */

namespace conddb {
   
  using Session = ora_wrapper::Session;
  //using IOVProxy = ora_wrapper::IOVProxy;
  using BaseKeyed = cond::BaseKeyed;

  class KeyList {
  public:
    typedef BaseKeyed Base;
    
    explicit KeyList( Session& session );

    void init(cond::IOVProxy const & seq ) {
      m_sequence = seq;
    }

    void load(const std::string& tag, const std::vector<unsigned long long>& keys);

    template<typename T> 
    T const * get(int n) const {
      return dynamic_cast<T const *>(elem(n));
    }

   template<typename T> 
    T const * get(char const * iname) const {
      return dynamic_cast<T const *>(elem(iname));
    }
    
    template<typename T> 
    T const * get(std::string const & iname) const {
      return dynamic_cast<T const *>(elem(iname));
    }
    
    BaseKeyed const * elem(int n) const;

    BaseKeyed const * elem(char const * iname) const;

    BaseKeyed const * elem(std::string const & iname) const;

    int size() const { return m_data.size();}

  private:
    // the full collection of keyed object
    Session m_session;
    // the current set
    std::vector<boost::shared_ptr<Base> > m_data;

  };


}

#endif
