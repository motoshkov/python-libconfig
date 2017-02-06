#include <boost/python.hpp>
#include <libconfig.h++>
#include <string>

using namespace boost::python;
using namespace libconfig;

class pyConfig
{
public:
    pyConfig ()
    {
        config = new Config ();
        config->setTabWidth(4);
    }

    ~pyConfig ()
    {
        delete config;
    }

    void read(object& file)
    {
        try {
            config->read(getFILEfromObj(file));
        } catch (const ParseException& e) {
            throwParseException(e);
        }
    }

    void write(object& file)
    {
        config->write(getFILEfromObj(file));
    }

    void readFile(const char * filename)
    {
        try {
            config->readFile(filename);
        }
        catch (const ParseException& e) {
            throwParseException(e);
        }
    }

    void writeFile ( const char * filename )
    {   config->writeFile ( filename );    }

#if (LIBCONFIGXX_VER_MAJOR >= 2) || \
    ((LIBCONFIGXX_VER_MAJOR == 1) && (LIBCONFIGXX_VER_MINOR >= 4))
    void readString ( const char * stringdata )
    {   config->readString ( stringdata ); }
#endif

    bool getAutoConvert ()
    {   return config->getAutoConvert (); }

    void setAutoConvert ( bool flag )
    {   config->setAutoConvert ( flag );  }

    bool exists(const char * path)
    {
        if (path == NULL)
            return false;
        return config->exists(path);
    }

    tuple value ( const char * path )
    {   
        std::string v_string;
        if ( config->lookupValue ( path, v_string ) )
            return make_tuple ( v_string.c_str(), true );

        int v_int;
        if ( config->lookupValue ( path, v_int ) )
            return make_tuple ( v_int, true );

        unsigned int v_uint;
        if ( config->lookupValue ( path, v_uint ) )
            return make_tuple ( v_uint, true );

        bool v_bool;
        if ( config->lookupValue ( path, v_bool ) )
            return make_tuple ( v_bool, true );

        unsigned long long v_ulonglong;
        if ( config->lookupValue ( path, v_ulonglong ) )
            return make_tuple ( v_ulonglong, true );

        long long v_longlong;
        if ( config->lookupValue ( path, v_longlong ) )
            return make_tuple ( v_longlong, true );

        float v_float;
        if ( config->lookupValue ( path, v_float ) )
            return make_tuple ( v_float, true );

        double v_double;
        if ( config->lookupValue ( path, v_double ) )
            return make_tuple ( v_double, true );

        return make_tuple ( "", false );
    }

    list children_root ()
    {
        list result;
        int length = config->getRoot ().getLength ();
        for ( int index = 0; index < length; index++ )
        {
            result.append ( config->getRoot () [ index ].getPath ().c_str () );
        }
        return result;
    }

    list children ( const char * path )
    {
        list result;
        try {
            list result;
            int length = config->lookup ( path ).getLength ();
            for ( int index = 0; index < length; index++ )
            {
                result.append ( config->lookup ( path ) [ index ].getPath ().c_str () );
            }
            return result;
        }
        catch ( SettingNotFoundException & e )
        {
            return result;
        }
    }

    void remove ( const char * path, const char * name )
    {
        config->lookup ( path ).remove ( name );
    }

    void removeFromList ( const char * path, unsigned int id )
    {
        config->lookup ( path ).remove ( id );
    }

    void addBoolean ( const char * path, const char * name, bool value )
    {
        config->lookup ( path ).add ( name, libconfig::Setting::TypeBoolean ) = value;
    }

    void addBigInteger ( const char * path, const char * name, long long value )
    {
        config->lookup ( path ).add ( name, libconfig::Setting::TypeInt64 ) = value;
    }

    void addInteger ( const char * path, const char * name, int value )
    {
        config->lookup ( path ).add ( name, libconfig::Setting::TypeInt ) = value;
    }

    void addFloat ( const char * path, const char * name, double value )
    {
        config->lookup ( path ).add ( name, libconfig::Setting::TypeFloat ) = value;
    }

    void addString ( const char * path, const char * name, const char* value )
    {
        config->lookup ( path ).add ( name, libconfig::Setting::TypeString ) = value;
    }

    void addGroup ( const char * path, const char * name )
    {
        config->lookup ( path ).add ( name, libconfig::Setting::TypeGroup );
    }

    void addList ( const char * path, const char * name )
    {
        config->lookup ( path ).add ( name, libconfig::Setting::TypeList );
    }

    void addArray ( const char * path, const char * name )
    {
        config->lookup ( path ).add ( name, libconfig::Setting::TypeArray );
    }

    std::string appendBooleanToList ( const char * path, bool value )
    {
        return appendToList(path, libconfig::Setting::TypeBoolean, value);
    }

    std::string appendBigIntegerToList ( const char * path, long long value )
    {
        return appendToList(path, libconfig::Setting::TypeInt64, value);
    }

    std::string appendIntegerToList ( const char * path, int value )
    {
        return appendToList(path, libconfig::Setting::TypeInt, value);
    }

    std::string appendFloatToList ( const char * path, double value )
    {
        return appendToList(path, libconfig::Setting::TypeFloat, value);
    }

    std::string appendStringToList ( const char * path, const char* value )
    {
        return appendToList(path, libconfig::Setting::TypeString, value);
    }

    std::string appendGroupToList ( const char * path )
    {
        return config->lookup ( path ).add ( libconfig::Setting::TypeGroup ).getPath();
    }

    std::string appendArrayToList ( const char * path )
    {
        return config->lookup ( path ).add ( libconfig::Setting::TypeArray ).getPath();
    }

    std::string appendListToList ( const char * path )
    {
        return config->lookup ( path ).add ( libconfig::Setting::TypeList ).getPath();
    }


    void setValue_bool ( const char * path, bool value )
    {
        config->lookup ( path ) = value;
    }

    void setValue_int ( const char * path, int value )
    {
        libconfig::Setting &setting = config->lookup ( path );
        if (setting.isNumber()) {
            setting  = value;
        } else {
            setting = (value) ? true : false;
        }
    }

    void setValue_long ( const char * path, long value )
    {
        config->lookup ( path ) = value;
    }

    void setValue_longlong ( const char * path, long long value )
    {
        config->lookup ( path ) = value;
    }

    void setValue_float ( const char * path, float value )
    {
        config->lookup ( path ) = value;
    }

    void setValue_double ( const char * path, double value )
    {
        config->lookup ( path ) = value;
    }

    void setValue_str ( const char * path, std::string value )
    {
        config->lookup ( path ) = value;
    }

    void setIncludeDir (const char *includeDir)
    {
        config->setIncludeDir(includeDir);
    }

    const char * getIncludeDir ()
    {
        return config->getIncludeDir();
    }

    Setting::Type getType(const char* path)
    {
        return config->lookup(path).getType();
    }

private:
    Config * config;

    void throwParseException(const ParseException& e)
    {
        throw std::runtime_error(std::string("ParseException: Parse error at ") + e.getFile() + ":" + std::to_string(e.getLine()) + " - " + e.getError());
    }

    template<typename T>
    std::string appendToList(const char * path, Setting::Type type, const T& value)
    {
        Setting& s = config->lookup(path).add(type);
        s = value;
        return s.getPath();
    }

    FILE* getFILEfromObj(const object& o)
    {
        PyObject* p = o.ptr();
        if (PyFile_Check(p)) {
            return PyFile_AsFile(p);
        }
        else {
            throw std::runtime_error("Not a file object");
        }
    }
};

BOOST_PYTHON_MODULE ( pylibconfig )
{
    class_<pyConfig>("Config")
        .def("read", &pyConfig::read )
        .def("write", &pyConfig::write )
        .def("readFile", &pyConfig::readFile )
        .def("writeFile", &pyConfig::writeFile )
#if (LIBCONFIGXX_VER_MAJOR >= 2) || \
    ((LIBCONFIGXX_VER_MAJOR == 1) && (LIBCONFIGXX_VER_MINOR >= 4))
        .def("readString", &pyConfig::readString)
#endif
        .def("getAutoConvert", &pyConfig::getAutoConvert )
        .def("setAutoConvert", &pyConfig::setAutoConvert )
        .def("exists", &pyConfig::exists )
        .def("children", &pyConfig::children )
        .def("children", &pyConfig::children_root )
        .def("value", &pyConfig::value )
        .def("remove", &pyConfig::remove )
        .def("removeFromList", &pyConfig::removeFromList )
        .def("addString", &pyConfig::addString )
        .def("addBoolean", &pyConfig::addBoolean )
        .def("addBigInteger", &pyConfig::addBigInteger )
        .def("addInteger", &pyConfig::addInteger )
        .def("addFloat", &pyConfig::addFloat )
        .def("addGroup", &pyConfig::addGroup )
        .def("addList", &pyConfig::addList )
        .def("addArray", &pyConfig::addArray )
        .def("setValue", &pyConfig::setValue_bool )
        .def("setValue", &pyConfig::setValue_float )
        .def("setValue", &pyConfig::setValue_double )
        .def("setValue", &pyConfig::setValue_longlong )
        .def("setValue", &pyConfig::setValue_long )
        .def("setValue", &pyConfig::setValue_int )
        .def("setValue", &pyConfig::setValue_str )
        .def("appendBooleanToList", &pyConfig::appendBooleanToList )
        .def("appendBigIntegerToList", &pyConfig::appendBigIntegerToList )
        .def("appendIntegerToList", &pyConfig::appendIntegerToList )
        .def("appendFloatToList", &pyConfig::appendFloatToList )
        .def("appendStringToList", &pyConfig::appendStringToList )
        .def("appendArrayToList", &pyConfig::appendArrayToList )
        .def("appendGroupToList", &pyConfig::appendGroupToList )
        .def("appendListToList", &pyConfig::appendListToList )
        .def("setIncludeDir", &pyConfig::setIncludeDir )
        .def("getIncludeDir", &pyConfig::getIncludeDir )
        .def("getType", &pyConfig::getType )
    ;

    enum_<Setting::Type>("Type")
         .value("None", Setting::Type::TypeNone)
         .value("Int", Setting::Type::TypeInt)
         .value("Int64", Setting::Type::TypeInt64)
         .value("Float", Setting::Type::TypeFloat)
         .value("String", Setting::Type::TypeString)
         .value("Boolean", Setting::Type::TypeBoolean)
         .value("Group", Setting::Type::TypeGroup)
         .value("Array", Setting::Type::TypeArray)
         .value("List", Setting::Type::TypeList)
    ;
}
