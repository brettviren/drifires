#ifndef DRIFIRES_FACTORY_HPP
#define DRIFIRES_FACTORY_HPP

#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <map>

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
        using pointer = std::shared_ptr<Type>;
        using maker_f = std::function<Type*()>;

        Factory(std::string tname)
            : m_typename{tname}
            , m_maker{[] { return new Type; }}
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
                pointer p(new Type);
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
}


#endif

