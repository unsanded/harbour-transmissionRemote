#include "xmlrpcconnection.h"

XmlRpcConnection::XmlRpcConnection(QString url, QObject *parent) :
   RpcConnection(url, parent)
{
}



QVariant parseXmlVariant(const QDomElement& element)
{
    if(element.isText()){
        return element.toText().data();
    }
    //integer
    if(
            element.nodeName() == "i4" ||
            element.nodeName() == "i8" ||
            element.nodeName() == "int")
        return element.firstChild().toText().data().toLongLong();

    //boolean
    if(element.nodeName() == "boolean")
        return element.firstChild().toText().data() == "1";

    //string
    if(element.nodeName() == "string")
        return element.firstChild().toText().data();

    //dateTime
    if(element.nodeName() == "dateTime.iso8601")
        return QDateTime::fromString(element.firstChild().toText().data(),"yyyyMMddThh:mm:ss");

    if(element.nodeName() == "base64"){
        QString data = element.firstChild().toText().data();
        return QByteArray::fromBase64(data.toStdString().c_str());
    }
    if (element.nodeName() == "nil")
        return QVariant();

    //next come the big type with n children
    int childCount =  element.childNodes().count();

    if(element.nodeName() == "array"){
        QVariantList result;
        for (int i = 0; i< childCount; i++){
            result << parseXmlVariant(element.childNodes().at(i).toElement());
        }
        return result;
    }


    if(element.nodeName() == "struct"){
        QVariantMap result;

        for (int i = 0; i< childCount; i++){
            QString name = element.childNodes().at(i).namedItem("name").toText().data();

            result.insert(
                            name,
                            parseXmlVariant(element.childNodes().at(i).namedItem("value").toElement())
                        );
        }
        return result;

    }

    return QVariant(); //TODO throw exception
}




void XmlRpcCommand::gotReply()
{
    QDomDocument data("response");
    data.setContent(networkReply->readAll());
    QDomNode responseData = data.documentElement().elementsByTagName("methodResponse").at(0);

    QDomNode params = responseData.toElement().namedItem("params");

    if(params.isNull()){
        QDomNode fault = responseData.namedItem("fault");
        if(fault.isNull()){
            result = "RPC ERROR";
            qWarning() << "There were neither params nor a fault provided in the response to command: " << method;
            return;
        }
        result = "ERROR";
        QDomElement strukt = fault.toElement().elementsByTagName("struct").at(0).toElement();
        replyArguments=parseXmlVariant(strukt).toMap();
        return;
    }
    result = "succes";
    int paramCount = params.childNodes().size();



    QVariantList paramList = QVariantList();

    for(int i = 0; i < paramCount; i++){
        QDomNode paramValue = params.childNodes().at(i).toElement().firstChild();
        if(paramValue.nodeName() != "value") return;//TODO make some nice exception
        replyArguments[QString("%1").arg(i, 4)] = parseXmlVariant(paramValue.toElement());
    }
}

QDomElement buildDomElement(QVariant input,  QDomDocument& doc){
    QDomElement elem;
    switch(input.type()){
    case QVariant::UInt:
    case QVariant::Int:
    case QVariant::ULongLong:
    case QVariant::LongLong:
        elem = doc.createElement("int");
        elem.appendChild(doc.createTextNode(QString::number(input.toLongLong())));
        return elem;
    case QVariant::Double:
        elem = doc.createElement("double");
        elem.appendChild(doc.createTextNode(QString::number(input.toDouble())));
        return elem;
    case QVariant::String:
    case QVariant::Url:
        elem = doc.createElement("string");
        elem.appendChild(doc.createTextNode(input.toString()));
        return elem;
    case QVariant::Bool:
        elem = doc.createElement("boolean");
        elem.appendChild(doc.createTextNode(input.toBool()?"1":"0"));
        return elem;

    case QVariant::ByteArray:
        elem = doc.createElement("base64");
        elem.appendChild(doc.createTextNode(input.toByteArray().toBase64()));
        return elem;
    case QVariant::Invalid:
        elem = doc.createElement("nil");
        return elem;
    case QVariant::Map:
    {
        QVariantMap map = input.toMap();
        elem = doc.createElement("struct");
        for(typename QVariantMap::iterator iter = map.begin(); iter != map.end(); iter++){
            QDomElement name = doc.createElement("name");
            name.appendChild(doc.createTextNode(iter.key()));
            QDomElement value = doc.createElement("value");
            value.appendChild(buildDomElement(iter.value(), doc));

        }
        return elem;
    }
    case QVariant::List:
    case QVariant::StringList:
    {
        QVariantList list = input.toList();

        elem = doc.createElement("Array");
        QDomElement data = doc.createElement("data");
        elem.appendChild(data);


        for(typename QVariantList::iterator iter = list.begin(); iter != list.end(); iter++){
            QDomElement value = doc.createElement("value");
            value.appendChild(buildDomElement(*iter, doc));
            data.appendChild(value);
        }
        return elem;
    }
    case QVariant::Time:
    case QVariant::DateTime:
    case QVariant::Date:
        {
            elem = doc.createElement("dateTime.iso8601");
            QString content = input.toDateTime().toString("yyyyMMddThh:mm:ss");
            elem.appendChild(doc.createTextNode(content));
            return elem;
        }
    default:
        elem = doc.createElement("nil");
        qWarning() << "unsupported type " << input.typeName();
        return elem;
    }
}

QByteArray XmlRpcCommand::make()
{
    QDomDocument document;
    QDomElement methodCall = document.createElement("methodCall");
    document.appendChild(methodCall);

    QDomElement methodName=document.createElement("methodName");
    methodName.appendChild(document.createTextNode(method));
    methodCall.appendChild(methodName);
    QDomElement params = document.createElement("params");
    for (QVariant iter : requestArguments){
        QDomElement param = document.createElement("param");
        param.appendChild(buildDomElement(iter, document));
        params.appendChild(param);
    }
    return requestBlob;
}

void XmlRpcCommand::handleReply()
{
}
