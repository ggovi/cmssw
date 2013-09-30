#include "CondCore/CondDB/interface/Exception.h"
#include "SessionImpl.h"
//
#include "RelationalAccess/ITransaction.h"

namespace conddb {

SessionImpl::SessionImpl():
  configuration(),
  connectionService(),
  coralSession(){
}

SessionImpl::~SessionImpl(){
}
    
void SessionImpl::connect( const std::string& connectionString, 
			   bool readOnly ){
  disconnect();
  configuration.configure( connectionService.configuration() );
  coralSession.reset( connectionService.connect( connectionString, readOnly?coral::ReadOnly:coral::Update ) );
}
void SessionImpl::connect( const std::string& connectionString, 
			   const std::string&,
			   bool readOnly ){
  connect( connectionString, readOnly );
}

void SessionImpl::connect( boost::shared_ptr<coral::ISessionProxy>& csession ){
  disconnect();
  if( !configuration.isConfigured() ) configuration.configure( connectionService.configuration() );  
  coralSession = csession;
}

void SessionImpl::disconnect(){
  if( coralSession.get() ){
    if( coralSession->transaction().isActive() ){
      coralSession->transaction().rollback();
      transactionClients = 0;
    }
    coralSession.reset();
  }
  transactionCache.reset();
}
    
bool SessionImpl::isActive() const {
  return coralSession.get();
}

void SessionImpl::startTransaction( bool readOnly ){
  if( !transactionClients ){ 
    coralSession->transaction().start( readOnly );
    transactionCache.reset( new TransactionCache );
  } else {
    if(!readOnly ) throwException( "An update transaction is already active.",
				   "SessionImpl::startTransaction" );
  }
  transactionClients++;
}
    
void SessionImpl::commitTransaction(){
  if( transactionClients ) {
    transactionClients--;
    if( !transactionClients ){
      coralSession->transaction().commit();
      transactionCache.reset();
    }
  }
}
    
void SessionImpl::rollbackTransaction(){
  coralSession->transaction().rollback();
  transactionCache.reset();
  transactionClients = 0;
}
   
bool SessionImpl::isTransactionActive() const {
  if( !coralSession.get() ) return false;
  return coralSession->transaction().isActive();
}
 
coral::ISchema& SessionImpl::coralSchema(){
  if( !coralSession.get() ){
    conddb::throwException("The session is not active.","SessionImpl::coralSchema");
  }
  return coralSession->nominalSchema();
}

}
