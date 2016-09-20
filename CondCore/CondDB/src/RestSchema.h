#ifndef CondCore_CondDB_RestSchema_h
#define CondCore_CondDB_RestSchema_h

#include "CondCore/CondDB/interface/Types.h"
#include "CondCore/CondDB/interface/Time.h"
#include "CondCore/CondDB/interface/Binary.h"

#include "IDbSchema.h"
//
#include "ICondTransaction.h"
//#include "RelationalAccess/ITransaction.h"
//
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/property_tree/ptree.hpp>

#include <iostream>

#include <cpr/cpr.h>
#include "base64.h"


namespace cond {

  namespace persistency {
 
    
    static const std::string SERVICE_PATH = "/phycdb/v1/rest";

    class RestTransaction;

    class RestSession {
    public:
      RestSession( const std::string& connStr ) {
        m_url = connStr + SERVICE_PATH;
        //m_client.reset ( new http::client() ); // No more cpp-netlib.
      }
      ~RestSession(){}
      
      void setTransaction(std::shared_ptr<RestTransaction>& transaction) { m_transaction = transaction; }
      RestTransaction& transaction() { return *m_transaction; }
      //std::shared_ptr<http::client>& getClient() { return m_client; }
      const std::string getUrl() { return m_url; } 
    
    private:
      std::string m_url = "";
      //std::shared_ptr<http::client> m_client;
      std::shared_ptr<RestTransaction> m_transaction;
    };


    class RestTransaction : public ICondTransaction {
    public:
      RestTransaction( const std::shared_ptr<RestSession>& restSession ):
        m_session( restSession ){
      } 
      
      virtual ~RestTransaction(){}
    
      void start( bool rO ) { readOnly = rO; active = true; }
 
      void commit(){
        //m_session->transaction().commit();
      }
      
      void rollback(){
        //m_session->transaction().rollback();
      }

      bool isActive() const {
        return active; //m_session->transaction().isActive();
      }

      bool isReadOnly() const {
        return readOnly;
      }

    private: 
      std::shared_ptr<RestSession> m_session;
    };


    // IOV Schema

    class TagService : public ITagTable {
    public:
	explicit TagService( std::shared_ptr<RestSession>& session );
	virtual ~TagService(){}
	bool exists();
	void create();
	bool select( const std::string& name );
	bool select( const std::string& name, cond::TimeType& timeType, std::string& objectType, cond::SynchronizationType& synchronizationType,
		     cond::Time_t& endOfValidity, std::string& description, cond::Time_t& lastValidatedTime );
	bool getMetadata( const std::string& name, std::string& description, 
			  boost::posix_time::ptime& insertionTime, boost::posix_time::ptime& modificationTime );
	void insert( const std::string& name, cond::TimeType timeType, const std::string& objectType, 
		     cond::SynchronizationType synchronizationType, cond::Time_t endOfValidity, const std::string& description, 
		     cond::Time_t lastValidatedTime, const boost::posix_time::ptime& insertionTime );
	void update( const std::string& name, cond::SynchronizationType synchronizationType, cond::Time_t& endOfValidity, const std::string& description, 
		     cond::Time_t lastValidatedTime, const boost::posix_time::ptime& updateTime );
	void updateValidity( const std::string& name, cond::Time_t lastValidatedTime, const boost::posix_time::ptime& updateTime );
	void setValidationMode(){}
      private:
        void insert( const std::string& name, cond::TimeType timeType, const std::string& objectType,
                     cond::SynchronizationType synchronizationType, cond::Time_t endOfValidity, const std::string& description,
                     cond::Time_t lastValidatedTime, const boost::posix_time::ptime& insertionTime, 
                     const boost::posix_time::ptime& updateTime );
	std::shared_ptr<RestSession>& m_session;

	const std::string M_TAG_PATH   = "/tags";
        const std::string M_TAGNAME        = "name";
        const std::string M_TIMETYPE       = "timeType";
	const std::string M_OBJTYPE        = "objectType";
	const std::string M_SYNCTYPE       = "synchronization";
 	const std::string M_EOFVAL         = "endOfValidity";
	const std::string M_DESC           = "description";
	const std::string M_LAST_VAL       = "lastValidatedTime";
	const std::string M_INSERTIONTIME  = "insertionTime";
	const std::string M_MODTIME        = "modificationTime";

    };
     
    class PayloadService : public IPayloadTable {
    public:
	explicit PayloadService( std::shared_ptr<RestSession>& session );
	virtual ~PayloadService(){}
	bool exists();
	void create();
	bool select( const cond::Hash& payloadHash);
	bool select( const cond::Hash& payloadHash, std::string& objectType, 
		     cond::Binary& payloadData, cond::Binary& streamerInfoData);
	bool getType( const cond::Hash& payloadHash, std::string& objectType );
	bool insert( const cond::Hash& payloadHash, const std::string& objectType, 
		     const cond::Binary& payloadData, const cond::Binary& streamerInfoData, 
		     const boost::posix_time::ptime& insertionTime);
	cond::Hash insertIfNew( const std::string& objectType, const cond::Binary& payloadData, 
				const cond::Binary& streamerInfoData, const boost::posix_time::ptime& insertionTime );
      private:
	std::shared_ptr<RestSession>& m_session;

        const std::string M_PAYLOAD_PATH  = "/payloads";
        const std::string M_HASH    = "hash";
        const std::string M_DATA    = "data";
        const std::string M_TYPE    = "objectType";
        const std::string M_SINFO   = "streamerInfo";
        const std::string M_VERSION = "version";
        const std::string M_TIME    = "insertionTime";

    }; 
     
    class IovService : public IIOVTable {
    public:
	explicit IovService( std::shared_ptr<RestSession>& session );
	virtual ~IovService(){}
	bool exists();
	void create();

	size_t selectGroups( const std::string& tag, std::vector<cond::Time_t>& groups );
	size_t selectSnapshotGroups( const std::string& tag, const boost::posix_time::ptime& snapshotTime, 
				     std::vector<cond::Time_t>& groups );
	size_t selectLatestByGroup( const std::string& tag, cond::Time_t lowerGroup, cond::Time_t upperGroup, 
				    std::vector<std::tuple<cond::Time_t,cond::Hash> >& iovs);
	size_t selectSnapshotByGroup( const std::string& tag, cond::Time_t lowerGroup, cond::Time_t upperGroup, 
				      const boost::posix_time::ptime& snapshotTime, 
				      std::vector<std::tuple<cond::Time_t,cond::Hash> >& iovs);
	size_t selectLatest( const std::string& tag, std::vector<std::tuple<cond::Time_t,cond::Hash> >& iovs);
        size_t selectSnapshot( const std::string& tag, const boost::posix_time::ptime& snapshotTime,
                               std::vector<std::tuple<cond::Time_t,cond::Hash> >& iovs);
        bool getLastIov( const std::string& tag, cond::Time_t& since, cond::Hash& hash );
	bool getSnapshotLastIov( const std::string& tag, const boost::posix_time::ptime& snapshotTime, cond::Time_t& since, cond::Hash& hash );
        bool getSize( const std::string& tag, size_t& size );
        bool getSnapshotSize( const std::string& tag, const boost::posix_time::ptime& snapshotTime, size_t& size );
	void insertOne( const std::string& tag, cond::Time_t since, cond::Hash payloadHash, const boost::posix_time::ptime& insertTime);
	void insertMany( const std::string& tag, const std::vector<std::tuple<cond::Time_t,cond::Hash,boost::posix_time::ptime> >& iovs );
	void erase( const std::string& tag );
      private:
        size_t fillGroupVector( const boost::property_tree::ptree& pt, std::vector<cond::Time_t>& groups );

	std::shared_ptr<RestSession>& m_session;
        
        const std::string M_IOV_PATH  = "/iovs";
        const std::string M_FUNC_SELECT_GROUPS = "/selectGroups";
        const std::string M_FUNC_SELECT_SNAPSHOT = "/selectSnapshot";
        const std::string M_FUNC_SELECT_IOVS = "/selectIovs";
        const std::string M_FUNC_GET_SIZE = "/getSize";
        const std::string M_SINCE     = "since";
        const std::string M_HASH      = "payloadHash";
        const std::string M_GROUPS    = "groups";

    };

    class TagLogService: public ITagLogTable {
      public:
        explicit TagLogService( std::shared_ptr<RestSession>& session );
        virtual ~TagLogService(){}
        bool exists();
        void create();
        void insert( const std::string& tag, const boost::posix_time::ptime& eventTime, const std::string& userName, const std::string& hostName,
                           const std::string& command, const std::string& action, const std::string& userText );

      private:
        std::shared_ptr<RestSession>& m_session;
    };

 
    class TagMigrationService : public ITagMigrationTable {
      public:
        explicit TagMigrationService( std::shared_ptr<RestSession>& session );
        virtual ~TagMigrationService(){}
        bool exists();
        void create();
        bool select( const std::string& sourceAccount, const std::string& sourceTag, std::string& tagName, int& statusCode);
        void insert( const std::string& sourceAccount, const std::string& sourceTag, const std::string& tagName, 
                     int statusCode, const boost::posix_time::ptime& insertionTime);
        void updateValidationCode( const std::string& sourceAccount, const std::string& sourceTag, int statusCode );
      private:
        std::shared_ptr<RestSession>& m_session;

        const std::string M_TAGMIGRATION_PATH  = "/tagmigrations"; //M_TABLE_NAME
        const std::string M_SOURCE_ACCOUNT     = "sourceAccount";
        const std::string M_SOURCE_TAG         = "sourceTag";
        const std::string M_TAG_NAME           = "tagName";
        const std::string M_STATUS_CODE        = "statusCode";
        const std::string M_INSERTION_TIME     = "insertTime";

    };

    class PayloadMigrationService : public IPayloadMigrationTable {
      public:
        explicit PayloadMigrationService( std::shared_ptr<RestSession>& session );
        virtual ~PayloadMigrationService(){}
        bool exists();
        void create();
        bool select( const std::string& sourceAccount, const std::string& sourceToken, std::string& payloadId );
        void insert( const std::string& sourceAccount, const std::string& sourceToken, const std::string& payloadId, 
                     const boost::posix_time::ptime& insertionTime);
        void update( const std::string& sourceAccount, const std::string& sourceToken, const std::string& payloadId,
                     const boost::posix_time::ptime& insertionTime);
      private:
        std::shared_ptr<RestSession>& m_session;
        
        const std::string M_PAYLOADMIGRATION_PATH = "/payloadMigrations";
        const std::string M_SOURCE_ACCOUNT = "sourceAccount";
        const std::string M_SOURCE_TOKEN   = "sourceToken";
        const std::string M_PAYLOAD_ID     = "payloadId";
        const std::string M_INSERTION_TIME = "insertionTime";
    };

    class RestSchema : public IIOVSchema {
    public: 
      explicit RestSchema( std::shared_ptr<RestSession>& session );
      virtual ~RestSchema(){}
      bool exists();
      bool create();
      ITagTable& tagTable();
      IIOVTable& iovTable();
      IPayloadTable& payloadTable();
      ITagLogTable& tagLogTable();
      ITagMigrationTable& tagMigrationTable();
      IPayloadMigrationTable& payloadMigrationTable();
      std::string parsePoolToken( const std::string& );
    private:
      TagService m_tagService;
      IovService m_iovService;
      PayloadService m_payloadService;
      TagLogService m_tagLogService;
      TagMigrationService m_tagMigrationService;
      PayloadMigrationService m_payloadMigrationService;
    }; 


    // GT Schema

    class GTService : public IGTTable {
    public:
      explicit GTService( std::shared_ptr<RestSession>& session );
      virtual ~GTService(){}
      bool exists();
      void create();
      bool select( const std::string& name);
      bool select( const std::string& name, cond::Time_t& validity, boost::posix_time::ptime& snapshotTime );
      bool select( const std::string& name, cond::Time_t& validity, std::string& description,
                   std::string& release, boost::posix_time::ptime& snapshotTime );
      void insert( const std::string& name, cond::Time_t validity, const std::string& description, const std::string& release,
                   const boost::posix_time::ptime& snapshotTime, const boost::posix_time::ptime& insertionTime );
      void update( const std::string& name, cond::Time_t validity, const std::string& description, const std::string& release,
                   const boost::posix_time::ptime& snapshotTime, const boost::posix_time::ptime& insertionTime );
    private:
      std::shared_ptr<RestSession>& m_session;

      const std::string M_GT_PATH     = "/globaltags";
      const std::string M_NAME        = "name";
      const std::string M_VALIDITY    = "validity";
      const std::string M_DESCRIPTION = "description";
      const std::string M_RELEASE     = "release";
      const std::string M_SNAP_TIME   = "snapshotTimeMilli";
      const std::string M_INSERT_TIME = "insertionTimeMilli";
    };
    
    // ???? static constexpr unsigned int PAYLOAD_HASH_SIZE = 40; 
    class GTMapService : public IGTMapTable {
    public:
      explicit GTMapService( std::shared_ptr<RestSession>& session );
      virtual ~GTMapService(){}
      bool exists();
      void create();
      bool select( const std::string& gtName, std::vector<std::tuple<std::string,std::string,std::string> >& tags );
      bool select( const std::string& gtName, const std::string& preFix, const std::string& postFix,
      std::vector<std::tuple<std::string,std::string,std::string> >& tags );
      void insert( const std::string& gtName, const std::vector<std::tuple<std::string,std::string,std::string> >& tags );
    private:
      std::shared_ptr<RestSession>& m_session;

      const std::string M_GTMAP_PATH = "/globaltagmaps";
      const std::string M_NAME     = "gtname";
      const std::string M_RECORD   = "record";
      const std::string M_LABEL    = "label";
      const std::string M_TAG_NAME = "tagName";
 
    };
 
    class RestGTSchema : public IGTSchema {
    public:
      explicit RestGTSchema( std::shared_ptr<RestSession>& session );
      virtual ~RestGTSchema(){}
      bool exists();
      void create();
      IGTTable& gtTable();
      IGTMapTable& gtMapTable();
    private:
      GTService m_gtService;
      GTMapService m_gtMapService;
    };


    //BASE64 DECODE AND ENCODE
    static const std::string base64_chars =
                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                 "abcdefghijklmnopqrstuvwxyz"
                 "0123456789+/";
    
    static inline bool is_base64(unsigned char c) {
      return (isalnum(c) || (c == '+') || (c == '/'));
    }

    inline std::string b64Decode(std::string const& encoded_string) {
      int in_len = encoded_string.size();
      int i = 0;
      int j = 0;
      int in_ = 0;
      unsigned char char_array_4[4], char_array_3[3];
      std::string ret;

      while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
          for (i = 0; i <4; i++)
          char_array_4[i] = base64_chars.find(char_array_4[i]);

          char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
          char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
          char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

          for (i = 0; (i < 3); i++)
            ret += char_array_3[i];
          i = 0;
        }
      }

      if (i) {
        for (j = i; j <4; j++)
          char_array_4[j] = 0;

        for (j = 0; j <4; j++)
          char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
      }

      return ret;
    }

  }
}

#endif

