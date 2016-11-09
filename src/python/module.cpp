#include <sstream>

#include <boost/python.hpp>

#include "DataLisp.h"
#include "Data.h"
#include "DataContainer.h"
#include "DataArray.h"
#include "DataGroup.h"
#include "SourceLogger.h"

using namespace DL;
namespace bpy = boost::python; 

bpy::tuple version()
{
    return bpy::make_tuple(DL_VERSION_MAJOR, DL_VERSION_MINOR);
}

//--------
class DataGroup_PY;
class DataArray_PY;
class Data_PY : public Data
{
public:
    Data_PY(const string_t& key = "") : Data(key) {}
    explicit Data_PY(const Data& d) : Data(d) {} 

    DataGroup_PY* getGroup_PY() const;
    void setGroup_PY(DataGroup_PY*);

    DataArray_PY* getArray_PY() const;
    void setArray_PY(DataArray_PY*);

    std::string str_PY() const;
};

//--------
class DataArray_PY : public DataArray
{
public:
	DataArray_PY() : DataArray() {}

	Data_PY at_PY(size_t i) const {return Data_PY(at(i));}
    void add_PY(const Data_PY& d) { add(d); }
	void set_PY(size_t i, const Data_PY& d) { set(i, d); }
};

//--
class DataGroup_PY : public DataGroup
{
public:
    DataGroup_PY() : DataGroup() {}

	void addData_PY(const Data_PY& data) { addData(data); }
	Data_PY at_PY(size_t i) const { return Data_PY(at(i)); }
	Data_PY getFromKey_PY(const string_t& str) const { return Data_PY(getFromKey(str)); }

    Data_PY getSpecial_PY(const bpy::object& b) const
    {
        bpy::extract<size_t> v(b);
        if(v.check())
            return at_PY(v());
        else
        {
            bpy::extract<std::string> str(b);

            if(str.check())
                return getFromKey_PY(str());
            else
                return Data_PY();// Error?
        }
    }

    bool hasSpecialKey_PY(const bpy::object& b) const
    {
        return getSpecial_PY(b).isValid();
    }

	bpy::list getAllFromKey_PY(const string_t& key) const
    {
        auto l = getAllFromKey(key);

        bpy::list pyl;
        for(auto v : l)
            pyl.append(Data_PY(v));

        return pyl;
    }

	bpy::list getNamedEntries_PY() const
    {
        auto l = getNamedEntries();
        bpy::list pyl;
        for(auto v : l)
            pyl.append(Data_PY(v));
        return pyl;
    }

	bpy::list getAnonymousEntries_PY() const
    {
        bpy::list pyl;
        for(size_t i = 0; i < anonymousCount(); ++i)
            pyl.append(Data_PY(at(i)));
        return pyl;
    }

	bpy::list getEntries_PY() const
    {
        auto l = getNamedEntries();
        bpy::list pyl;
        for(size_t i = 0; i < anonymousCount(); ++i)
            pyl.append(Data_PY(at(i)));
        for(auto v : l)
            pyl.append(Data_PY(v));
        return pyl;
    }
};

//-- (Data implementation)
DataGroup_PY* Data_PY::getGroup_PY() const { return (DataGroup_PY*)getGroup(); }
void Data_PY::setGroup_PY(DataGroup_PY* p) { setGroup(p); }

DataArray_PY* Data_PY::getArray_PY() const { return (DataArray_PY*)getArray(); }
void Data_PY::setArray_PY(DataArray_PY* p) { setArray(p); }

std::string Data_PY::str_PY() const
{
    std::stringstream stream;
    stream << "Data[" << key() << "](";
    switch(type())
    {
    case Data::T_None:
        stream << "None";
        break;
    case Data::T_Array:
        stream << "Array{" << getArray()->size() << "}";
        break;
    case Data::T_Group:
        stream << "Group{" << getGroup()->id() << "}";
        break;
    case Data::T_String:
        stream << getString();
        break;
    case Data::T_Integer:
        stream << getInt();
        break;
    case Data::T_Float:
        stream << getFloat();
        break;
    case Data::T_Bool:
        stream << (getBool() ? "True" : "False");
        break;
    }
    stream << ")";

    return stream.str();
}

//--
class DataContainer_PY : public DataContainer
{
public:
    DataContainer_PY() : DataContainer()
    {
    }

    bpy::list getTopGroups_PY() const
    {
        auto l = getTopGroups();

        bpy::list pyl;
        for(auto v : l)
            pyl.append((DataGroup_PY*)v);

        return pyl;
    }

    void addTopGroup_PY(DataGroup_PY* group) { addTopGroup((DataGroup*)group); }

    DataGroup_PY* createGroup_PY() { return (DataGroup_PY*)createGroup(); }
    DataArray_PY* createArray_PY() { return (DataArray_PY*)createArray(); }
};

//--
class DataLisp_PY : public DataLisp
{
public:
    DataLisp_PY(SourceLogger* log) : DataLisp(log) {}

	void build_PY(DataContainer_PY& container) { build(container); }
	static string_t generate_PY(const DataContainer_PY& container) { return generate(container); }
};

//----------
BOOST_PYTHON_MODULE(pydatalisp)
{
    bpy::def("version", version);

    //--
    bpy::class_<SourceLogger>("SourceLogger")
        .add_property("warningCount", &SourceLogger::warningCount)
        .add_property("errorCount", &SourceLogger::errorCount)
    ;

    //--// TODO: Add expressions!
    bpy::class_<DataLisp_PY>("DataLisp",
            bpy::init<SourceLogger*>(bpy::args("source_logger")))
        .def("parse", &DataLisp_PY::parse)
        .def("build", &DataLisp_PY::build_PY)
        .def("generate", &DataLisp_PY::generate_PY)
        .staticmethod("generate")
        .def("dump", &DataLisp_PY::dump)
    ;

    //--
    bpy::class_<DataContainer_PY>("DataContainer")
        .add_property("topGroups", &DataContainer_PY::getTopGroups_PY)
        .def("addTopGroup", &DataContainer_PY::addTopGroup_PY)
        .def("createGroup", &DataContainer_PY::createGroup_PY, bpy::return_internal_reference<>())
        .def("createArray", &DataContainer_PY::createArray_PY, bpy::return_internal_reference<>())
	;

    //--
    bpy::class_<DataGroup_PY>("DataGroup", bpy::no_init)
        .add_property("id", &DataGroup_PY::id, &DataGroup_PY::setID)
        .def("addData", &DataGroup_PY::addData_PY)
        .def("at", &DataGroup_PY::at_PY)
        .def("__getitem__", &DataGroup_PY::getSpecial_PY)
        .add_property("anonymousCount", &DataGroup_PY::anonymousCount)
        .def("fromKey", &DataGroup_PY::getFromKey_PY)
        .def("allFromKey", &DataGroup_PY::getAllFromKey_PY)
        .def("hasKey", &DataGroup_PY::hasKey)
        .def("__contains__", &DataGroup_PY::hasSpecialKey_PY)
        .add_property("namedEntries", &DataGroup_PY::getNamedEntries_PY)
        .add_property("anonymousEntries", &DataGroup_PY::getAnonymousEntries_PY)
        .add_property("entries", &DataGroup_PY::getEntries_PY)
    ;

    //--
    bpy::class_<DataArray_PY>("DataArray", bpy::no_init)
        .def("__len__", &DataArray_PY::size)
        .add_property("size", &DataArray_PY::size)
        .def("at", &DataArray_PY::at_PY)
        .def("__getitem__", &DataArray_PY::at_PY)
		.def("add", &DataArray_PY::add_PY)
		.def("set", &DataArray_PY::set_PY)
        .def("__setitem__", &DataArray_PY::set_PY)
        .def("isAllType", &DataArray_PY::isAllType)
		.def("isAllNumber", &DataArray_PY::isAllNumber)
    ;

    //--
    bpy::class_<Data_PY>("Data",
            bpy::init<bpy::optional<string_t> >(bpy::args("key")))
        .add_property("key", &Data_PY::key, &Data::setKey)
        .add_property("type", &Data_PY::type)
        .add_property("group", 
            bpy::make_function(&Data_PY::getGroup_PY, bpy::return_internal_reference<>()),
            &Data_PY::setGroup_PY)
        .add_property("array", 
            bpy::make_function(&Data_PY::getArray_PY, bpy::return_internal_reference<>()),
            &Data_PY::setArray_PY)
        .add_property("int", &Data_PY::getInt, &Data_PY::setInt)
        .add_property("float", &Data_PY::getFloat, &Data_PY::setFloat)
        .add_property("number", &Data_PY::getNumber)
        .add_property("bool", &Data_PY::getBool, &Data_PY::setBool)
        .add_property("string", &Data_PY::getString, &Data_PY::setString)
        //.add_property("object", &Data_PY::getObject_PY)
        .def("__str__", &Data_PY::str_PY)
        .def("__bool__", &Data_PY::isValid)
        .def("isNumber", &Data_PY::isNumber)
    ;

    bpy::enum_<Data::Type>("Type")
        .value("Group", Data::T_Group)
        .value("Integer", Data::T_Integer)
        .value("Float", Data::T_Float)
        .value("Bool", Data::T_Bool)
        .value("String", Data::T_String)
        .value("Array", Data::T_Array)
        .value("None", Data::T_None)
    ;
}