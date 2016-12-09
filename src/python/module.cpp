#include <sstream>

#include <boost/python.hpp>

#include "DataLisp.h"
#include "Data.h"
#include "DataContainer.h"
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
class Data_PY : public Data
{
public:
    Data_PY(const string_t& key = "") : Data(key) {}
    explicit Data_PY(const Data& d) : Data(d) {} 

    DataGroup_PY getGroup_PY() const;

    std::string str_PY() const;
};

//--------
class DataGroup_PY : public DataGroup
{
public:
    DataGroup_PY(const string_t& id = "") : DataGroup(id) {}
    explicit DataGroup_PY(const DataGroup& d) : DataGroup(d) {} 

	void add_PY(const Data_PY& data) { add(data); }
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
DataGroup_PY Data_PY::getGroup_PY() const
{
    return DataGroup_PY(getGroup());
}

std::string Data_PY::str_PY() const
{
    std::stringstream stream;
    stream << "Data[" << key() << "](";
    switch(type())
    {
    case Data::T_None:
        stream << "None";
        break;
    case Data::T_Group:
        if(getGroup().isArray())
            stream << "Array";
        else
            stream << "Group{" << getGroup().id() << "}";
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
            pyl.append(DataGroup_PY(v));

        return pyl;
    }

    void addTopGroup_PY(const DataGroup_PY& group) { addTopGroup(group); }
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
	;

    //--
    bpy::class_<DataGroup_PY>("DataGroup",
            bpy::init<bpy::optional<string_t> >(bpy::args("id")))
        .add_property("id", &DataGroup_PY::id, &DataGroup_PY::setID)
        .def("add", &DataGroup_PY::add_PY)
        .def("at", &DataGroup_PY::at_PY)
        .def("__getitem__", &DataGroup_PY::getSpecial_PY)
        .add_property("anonymousCount", &DataGroup_PY::anonymousCount)
        .add_property("isArray", &DataGroup_PY::isArray)
        .def("fromKey", &DataGroup_PY::getFromKey_PY)
        .def("allFromKey", &DataGroup_PY::getAllFromKey_PY)
        .def("hasKey", &DataGroup_PY::hasKey)
        .def("__contains__", &DataGroup_PY::hasSpecialKey_PY)
        .add_property("namedEntries", &DataGroup_PY::getNamedEntries_PY)
        .add_property("anonymousEntries", &DataGroup_PY::getAnonymousEntries_PY)
        .add_property("entries", &DataGroup_PY::getEntries_PY)
		.def("isAllAnonymousNumber", &DataGroup_PY::isAllNumber)
		.def("isAllNamedNumber", &DataGroup_PY::isAllNumber)
		.def("isAllNumber", &DataGroup_PY::isAllNumber)
    ;

    //--
    bpy::class_<Data_PY>("Data",
            bpy::init<bpy::optional<string_t> >(bpy::args("key")))
        .add_property("key", &Data_PY::key, &Data::setKey)
        .add_property("type", &Data_PY::type)
        .add_property("group", &Data_PY::getGroup_PY, &Data_PY::setGroup)
        .add_property("int", &Data_PY::getInt, &Data_PY::setInt)
        .add_property("float", &Data_PY::getFloat, &Data_PY::setFloat)
        .add_property("number", &Data_PY::getNumber)
        .add_property("bool", &Data_PY::getBool, &Data_PY::setBool)
        .add_property("string", &Data_PY::getString, &Data_PY::setString)
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
        .value("None", Data::T_None)
    ;
}