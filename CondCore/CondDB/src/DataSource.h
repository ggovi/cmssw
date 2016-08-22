#ifndef CondCore_CondDB_DataSource_h
#define CondCore_CondDB_DataSource_h

#include "CondCore/CondDB/interface/Types.h"

#include <iostream>
#include <memory>

#include "RelationalAccess/ITransaction.h"

namespace cond {

  namespace persistency {

    class DataSourceBase {
    public:
      virtual ~DataSourceBase() {}
      virtual coral::ITransaction& transaction() = 0;
      template<class T> std::shared_ptr<T>& getAs();
      template<class T, class U> void setAs(const std::shared_ptr<U>& rhs);
    };

    template <typename T>
    class DataSource : public DataSourceBase {
    public:
      DataSource(const std::shared_ptr<T>& ds) : m_ds(ds) {}
      std::shared_ptr<T>& getAs() { return m_ds; }
      void setAs(const std::shared_ptr<T>& rhs) { m_ds=rhs; }
      coral::ITransaction& transaction() { return m_ds->transaction(); }
    private:
      std::shared_ptr<T> m_ds;
    };

    template<class T> std::shared_ptr<T>& DataSourceBase::getAs()
    { return dynamic_cast<DataSource<T>&>(*this).getAs(); }

    template<class T, class U> void DataSourceBase::setAs(const std::shared_ptr<U>& rhs)
    { return dynamic_cast<DataSource<T>&>(*this).setAs(rhs); }

  }

}

#endif

