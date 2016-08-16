# xmlobj
C++ source code of a project which is convenient to convert between class (object) and XML string

Usage: Convert between xml object and xml string
===================================================================

example
-------------------------------------------------------------------

<code>
//-----------------------------------------------------------------
// define a xml document class (include sub class inherit iXmlNode)
// root node should implement iXmlDoc

#include "xml_adapter.h"
#include "ixml.h"

class example : public iXmlDoc {
public:
    void Serialize(xml::Adapter &adapter) {
        std::unordered_map<std::string, std::string> attrs;
        attrs["version"] = "1.0";
        attrs["encoding"] = "utf-8";
        // set meta data in root node
        // this is required, maps to "<?xml ... ?>"
        xml::Procinst(adapter, "xml", attrs);
        // this is required, maps to "<example> ... </example>"
        xml::Class xml(adapter, "example");
        XML_ATTR(adapter, attr1);
        XML_NODE(adapter, attr2);
        XML_NODE(adapter, values);
        XML_NODE(adapter, subnodes);
    }

public:
    std::string attr1;
    float attr2;
    std::vector<int> values;
    std::vector<subnode> subnodes;
};

class subnode : public iXmlNode {
public:
    bool attr;
    std::string content;

public:
    void Serialize(xml::Adapter &adapter) {
        // this is required, maps to "<subnode> ... </subnode>" 
        xml::Class xml(adapter, "subnode");
        XML::ATTR(adapter, attr);
        XML::NODE(adapter, content);
    }
};
</code>

How to call
------------------------------------------------------------------
<code>
//----------------------------------------------------------------
// how convert between xml object and xml strings
int main() {
    example rootNode;
    rootNode.attr1 = "value1";
    rootNode.attr2 = 100.0;
    rootNode.values.push_back(1);
    rootNode.values.push_back(2);
    subnode n1;
    n1.attr = true;
    n1.content = "sub node 1";
    rootNode.subnodes.push_back(n1);
    subnode n2;
    n2.attr = false;
    n2.content = "sub node 2";
    rootNode.subnodes.push_back(n2);

    // convert xml object to strings
    std::string xmlStr = xml::iXmlDoc::Marshal<example>(rootNode);
    
    // convert xml strings to xml object
    example sink;
    try {
        xml::iXmlDoc::UnMarshal<example>(xmlStr);
    } catch (xml::xml_exception &ex) {
        ...
    }
}
</code>

converted xml strings like as follow:
--------------------------------------------------------------------
<code>
<?xml version="1.0" encoding="utf-8"?>
<example attr1="value1">
    <attr2>100.0</attr2>
    <values>1</values>
    <values>2</values>
    <subnode attr="true">sub node 1<subnode>
    <subnode attr="false">sub node 2<subnode>
</example>
</code>

Nodes
--------------------------------------------------------------------
support type: 
    * std::string
    * int
    * unsigned in
    * long
    * unsinged char
    * double
    * bool
    * std::vector
    * class implemented xml::iXmlNode
