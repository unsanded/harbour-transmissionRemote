#include "xmlrpcconnection.h"

class DialectCommand : public XmlRpcCommand{
public:
    DialectCommand(QObject* parent):
    XmlRpcCommand("xmlrpc_dialect"){
        requestArguments.append("i8");
    }
    void handleReply(){
        //deleteLater();
    }
};

XmlRpcConnection::XmlRpcConnection(QString url, QObject *parent) :
   RpcConnection(url, parent)
{
    contentType="application/xml";
    //XmlRpcCommand* dialectCommand = new DialectCommand(this);

    //sendCommand(dialectCommand);
}



QVariant parseXmlVariant(const QDomNode& e)
{
    if(e.isText()){
        return e.toText().data();
    }
    QDomNode element = e.toElement();
    QString nodename = element.nodeName();

    //integer
    if(
            nodename == "i4" ||
            nodename == "i8" ||
            nodename == "int")
        return element.firstChild().toText().data().toLongLong();

    //boolean
    if(nodename == "boolean")
        return element.firstChild().toText().data() == "1";

    //string
    if(nodename == "string")
        return element.firstChild().toText().data();

    //dateTime
    if(nodename == "dateTime.iso8601")
        return QDateTime::fromString(element.firstChild().toText().data(),"yyyyMMddThh:mm:ss");

    if(nodename == "base64"){
        QString data = element.firstChild().toText().data();
        return QByteArray::fromBase64(data.toStdString().c_str());
    }
    if (nodename == "nil")
        return QVariant();

    //next come the big type with n children

    if(nodename == "array"){
        QVariantList result;
        QDomElement data = element.childNodes().at(0).toElement();
        int childCount = data.childNodes().count();
        for (int i = 0; i< childCount; i++){
            //dont even ask...
            result << parseXmlVariant(data.childNodes().at(i).toElement().childNodes().at(0));
        }
        return result;
    }


    if(nodename == "struct"){
        QVariantMap result;
        int childCount =  element.childNodes().count();
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
    qDebug() << "got reply";
    QDomDocument data("response");
    data.setContent(networkReply->readAll());
    QDomElement responseData = data.documentElement().toElement();
    if(responseData.toElement().tagName()!= "methodResponse")
        qWarning() << "wrong tag name in response. got" << responseData.tagName() << "instead of 'methodResponse'";

    QDomNode params = responseData.namedItem("params");

    if(params.isNull()){
        QDomNode fault = responseData.namedItem("fault");
        if(fault.isNull()){
            result = "RPC ERROR";
            qWarning() << "There were neither params nor a fault provided in the response to command: " << method;
            qWarning() << data.toByteArray(2);
            return;
        }
        result = "ERROR";

        qWarning() << "xmlrpc call return an error:" << data.toByteArray(2);
        qWarning() << "request was" << requestBlob;

        //yes, i know... but struct is a keyword
        QDomElement strukt = fault.toElement().elementsByTagName("struct").at(0).toElement();
        replyArguments << parseXmlVariant(strukt).toMap();
        return;
    }
    result = "succes";
    int paramCount = params.childNodes().size();

    QVariantList paramList = QVariantList();

    for(int i = 0; i < paramCount; i++){
        QDomNode paramValue = params.childNodes().at(i).toElement().firstChild();
        if(paramValue.nodeName() != "value") return;//TODO make some nice exception
        replyArguments.append(parseXmlVariant(paramValue.toElement().childNodes().at(0)));
    }
    handleReply();
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
    document.appendChild(document.createProcessingInstruction("xml", "version=\"1.0\" encoding='UTF-8'"));
    QDomElement methodCall = document.createElement("methodCall");
    document.appendChild(methodCall);

    QDomElement methodName=document.createElement("methodName");
    methodName.appendChild(document.createTextNode(method));
    methodCall.appendChild(methodName);
    QDomElement params = document.createElement("params");
    for (QVariant iter : requestArguments){
        QDomElement param = document.createElement("param");
        QDomElement value = document.createElement("value");
        param.appendChild(value);
        value.appendChild(buildDomElement(iter, document));
        params.appendChild(param);
    }
    methodCall.appendChild(params);

    requestBlob = document.toByteArray();
    return requestBlob;
}
