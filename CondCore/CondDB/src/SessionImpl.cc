#include "CondCore/CondDB/interface/Exception.h"
#include "SessionImpl.h"
#include "DbConnectionString.h"
#include "NoSqlEngine.h"
#include "ICondTransaction.h"
//
//
#include "RelationalAccess/ISessionProxy.h"
#include "RelationalAccess/ITransaction.h"

namespace cond {

  namespace persistency {

    class CondDBTransaction : public ICondTransaction {
    public:
      CondDBTransaction( const std::shared_ptr<coral::ISessionProxy>& coralSession ):
	m_session( coralSession ){
      }
      virtual ~CondDBTransaction(){}
     
      void start( bool readOnly = true ){

      };

      void commit(){
	m_session->transaction().commit();
      }
      
      void rollback(){
	m_session->transaction().rollback();
      }

      bool isActive(){
	return m_session->transaction().isActive();
      }

      bool isActive() const {
        return m_session->transaction().isActive();
      }

      bool isReadOnly() const { /* FIXME: RS */
        //throwException( "Under construction... ", "CondDBTransaction::isReadOnly");
        return false;
      }
    private: 
      std::shared_ptr<coral::ISessionProxy> m_session;
    };


    SessionImpl::SessionImpl():
      coralSession(),
      theBackendType( UNKNOWN_DB ) {
    }

    SessionImpl::SessionImpl( std::shared_ptr<coral::ISessionProxy>& session, 
			      const std::string& connectionStr,
                              BType backType ):
      //coralSession( session ),
      connectionString( connectionStr ),
      theBackendType( backType ){

      coralSession.reset( new DataSource<coral::ISessionProxy>( session ) );
    }

    /* DataSource constructed. New, generic one. */
    SessionImpl::SessionImpl( std::shared_ptr<DataSourceBase>& session,
                              const std::string& connectionStr,
                              BType backType ):
      connectionString( connectionStr),
      theBackendType( backType ) {
      coralSession = session;
    } 

    SessionImpl::~SessionImpl(){
      close();
    }

    void SessionImpl::close(){
      if( coralSession.get() ){
	if( coralSession->transaction().isActive() ){
	  coralSession->transaction().rollback();
	}
	coralSession.reset();
      }
      transaction.reset();
    }
    
    bool SessionImpl::isActive() const {
      return coralSession.get();
    }

    void SessionImpl::startTransaction( bool readOnly ){
      if( !transaction.get() ){
        if ( theBackendType == COND_DB ) {
          std::shared_ptr<coral::ISessionProxy>& cSess = coralSession->getAs<coral::ISessionProxy>();
  	  coralSession->transaction().start( readOnly );
	  iovSchemaHandle.reset( new IOVSchema( cSess->nominalSchema() ) );
	  gtSchemaHandle.reset( new GTSchema( cSess->nominalSchema() ) );
	  transaction.reset( new CondDBTransaction( cSess ) );
      
        } else if ( theBackendType == REST ) {
          NoSqlEngine::resetAs<RestSession, RestSchema, RestGTSchema, RestTransaction>(
              coralSession, iovSchemaHandle, gtSchemaHandle, transaction, readOnly );
        } else {
          throwException( "No valid database found.", "SessionImpl::startTransaction" );
        }
      } else {
	  if(!readOnly ) throwException( "An update transaction is already active.",
	   			         "SessionImpl::startTransaction" );
      }
      transaction->clients++;
    }
    
    void SessionImpl::commitTransaction(){
      if( transaction ) {
	transaction->clients--;
	if( !transaction->clients ){
	  transaction->commit();
	  transaction.reset();
	  iovSchemaHandle.reset();
	  gtSchemaHandle.reset();
	}
      }
    }
    
    void SessionImpl::rollbackTransaction(){
      if( transaction ) {   
	transaction->rollback();
	transaction.reset();
	iovSchemaHandle.reset();
	gtSchemaHandle.reset();
      }
    }
    
    bool SessionImpl::isTransactionActive( bool deep ) const {
      if( !transaction ) return false;
      if( !deep ) return true;
      return transaction->isActive();
    }

    void SessionImpl::openIovDb( SessionImpl::FailureOnOpeningPolicy policy ){
      if(!transaction.get()) throwException( "The transaction is not active.","SessionImpl::openIovDb" );
      if( !transaction->iovDbOpen ){
	transaction->iovDbExists = iovSchemaHandle->exists();
	transaction->iovDbOpen = true;
      }      
      if( !transaction->iovDbExists ){
	if( policy==CREATE ){
	  iovSchemaHandle->create();
	  transaction->iovDbExists = true;
	} else {
	  if( policy==THROW) throwException( "IOV Database does not exist.","SessionImpl::openIovDb");
	}
      }
    }

    void SessionImpl::openGTDb( SessionImpl::FailureOnOpeningPolicy policy ){
      if(!transaction.get()) throwException( "The transaction is not active.","SessionImpl::open" );
      if( !transaction->gtDbOpen ){
	transaction->gtDbExists = gtSchemaHandle->exists();
	transaction->gtDbOpen = true;
      }
      if( !transaction->gtDbExists ){
        if( policy==CREATE ){
          gtSchemaHandle->create();
          transaction->gtDbExists = true;
        } else {
          if( policy==THROW) throwException( "GT Database does not exist.","SessionImpl::openGTDb");
	}
      }
    }

    void SessionImpl::openDb(){
      if(!transaction.get()) throwException( "The transaction is not active.","SessionImpl::openIovDb" );
      if( !transaction->iovDbOpen ){
        transaction->iovDbExists = iovSchemaHandle->exists();
        transaction->iovDbOpen = true;
      }
      if( !transaction->gtDbOpen ){
        transaction->gtDbExists = gtSchemaHandle->exists();
        transaction->gtDbOpen = true;
      }
      if( !transaction->iovDbExists ){
	iovSchemaHandle->create();
	transaction->iovDbExists = true;
	if( !transaction->gtDbExists ){
	  gtSchemaHandle->create();
	  transaction->gtDbExists = true;
	}
      }
    }
    
    IIOVSchema& SessionImpl::iovSchema(){
      return *iovSchemaHandle;
    }

    IGTSchema& SessionImpl::gtSchema(){
      return *gtSchemaHandle;
    }

    bool SessionImpl::isOra(){
      if(!transaction.get()) throwException( "The transaction is not active.","SessionImpl::open" );
      return transaction->isOra;
    }

  }
}
