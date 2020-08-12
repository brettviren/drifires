#ifndef DRIFIRES_FACTORY_HPP
#define DRIFIRES_FACTORY_HPP

#include <exception>
#include <functional>
#include <string>
#include <map>

#include "drifires/object.hpp"
#include "drifires/configurable.hpp"

namespace drifires {

    struct FactoryException : public virtual std::runtime_error {
        using std::runtime_error::runtime_error ;
    };

    class FactoryBase {
      public:
        virtual std::string type_name() const = 0;
    };

    template<class Type>
    class Factory : public FactoryBase {
      public:

        using type = Type;
        using pointer = Type*;
        using maker_f = std::function<Type*()>;

        Factory(std::string tname)
            : m_typename{tname}
            , m_maker{[]{ return nullptr; }}
        {}
        virtual ~Factory() {}

        virtual std::string type_name() const { return m_typename; }

        pointer find(const std::string& name) {
            auto it = m_objects.find(name);
            if (it == m_objects.end()) {
                return nullptr;
            }
            return it->second;
        }

        pointer get(const std::string& name="") {
            auto it = m_objects.find(name);
            if (it == m_objects.end()) {
                pointer p = m_maker();
                if (p == nullptr) {
                    throw FactoryException("maker failed for " + type_name() + ":" + name);
                }
                m_objects[name] = p;
                return p;
            }
            return it->second;
        }
        
        // A particular type may call at file scope to set custom
        // maker.  Type name is returned.
        std::string set_maker(maker_f maker) {
            m_maker = maker;
            return m_typename;
        }

      private:
        std::map<std::string, pointer> m_objects;
        std::string m_typename;
        maker_f m_maker;
    };

    // Access a factory.
    template<class Type>
    Factory<Type>& factory(std::string tname) {
        static std::map<std::string, FactoryBase*> facfac;
        FactoryBase* fb = facfac[tname];
        if (!fb) {
            facfac[tname] = fb = new Factory<Type>(tname);
        }
        Factory<Type>* ret = dynamic_cast<Factory<Type>*>(fb);
        if (!ret) {
            throw FactoryException("type mismatch for " + tname);
        }
        return *ret;
    }

    template<class Type>
    Type& factory_get(drifires::object tnobj) {
        auto tn = tnobj.get<TypeName>();
        Type* ret = factory<Type>(tn.type).get(tn.name);
        if (ret == nullptr) {
            throw FactoryException("no instance " + tn.type + ":" + tn.name);
        }
        return *ret;
    }

    template<class Type>
    Type& factory_getcfg(drifires::object cfg) {
        TypeName tn = cfg.get<TypeName>();
        Type* ret = factory<Type>(tn.type).get(tn.name);
        ConfigurableBase* cb = dynamic_cast<ConfigurableBase*>(ret);
        if (cb)
            cb->configure(cfg);
        return *ret;
    }

}


#endif

