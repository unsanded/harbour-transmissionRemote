#include "xmlrpcconnection.h"

XmlRpcConnection::XmlRpcConnection(QUrl server, QObject *parent) :
    RpcConnection(server, parent)
{
}



QVariant parseXmlVariant(const QDomElement& element)
{
    //integer
    if(element.nodeName() == "i4" || element.nodeName() == "int")
        return element.firstChild().toText().data().toInt();

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




void XmlRpcCommand::gotReply(const QDomDocument &data)
{
    QDomNode responseData = data.documentElement().elementsByTagName("methodResponse").at(0);

    QDomNode params = responseData.toElement().namedItem("params");
    if(params.isNull()){
        QDomNode fault = responseData.namedItem("fault");
        if(fault.isNull()){
            reply.result = "RPC ERROR";
            qWarning() << "There were neither params nor a fault provided in the response to command: " << request.method;
            return;
        }
        reply.result = "ERROR";
        QDomElement strukt = fault.toElement().elementsByTagName("struct").at(0).toElement();
        reply.arguments=parseXmlVariant(strukt).toMap();
        return;
    }
    reply.result = "succes";
    int paramCount = params.childNodes().size();



    QVariantList paramList = QVariantList();

    for(int i = 0; i < paramCount; i++){
        QDomNode paramValue = params.childNodes().at(i).toElement().firstChild();
        if(paramValue.nodeName() != "value") return;//TODO make some nice exception
        reply.arguments[QString("%1").arg(i, 4)] = parseXmlVariant(paramValue.toElement());
    }
}

QByteArray XmlRpcCommand::make()
{
}

void XmlRpcCommand::handleReply()
{
}
