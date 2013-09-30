#include "CondCore/CondDB/interface/KeyList.h"

namespace conddb {

  KeyList::KeyList( Session& session ) : m_session( session ),
					 m_data(){
  }
  
  void KeyList::load(const std::string& tag, const std::vector<unsigned long long>& keys ) {
    m_session.transaction().start(true);
    m_data.resize(keys.size());
    IOVProxy proxy = m_session.readIov( tag );
    for (size_t i=0; i<keys.size(); ++i) {
      m_data[i].reset();
      if (keys[i]!=0) {
        auto p = proxy.find( keys[i] );
	if ( p!=m_proxy.end() ) { 
	  m_data[i] = m_session.getTypedObject<Base>( (*p).token() );
	}
      }
    }
    m_sequence.db().transaction().commit();
  }


  BaseKeyed const * KeyList::elem(int n) const {
    if (!m_data[n]) return 0;
    return m_data[n].get();
  }
  

}
