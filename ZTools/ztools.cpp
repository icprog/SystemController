#include "ztools.h"
#include<QEventLoop>
#include<QTimer>
#include<ctype.h>
#include<QFile>
#include<unistd.h>
#include<QNetworkInterface>
#include<QStringList>
#include<QTextCodec>
#include <QMutex>
ZTools::ZTools()
{
}
QByteArray ZTools::readLineFromFile(const QString& fileName,int lineNo)
{
    QFile f(fileName);
    QByteArray line;
    f.open(QFile::ReadOnly);
    while(!f.atEnd()&&lineNo--)
        line = f.readLine();
    if (f.atEnd() && lineNo > 0)
        line = "";
    else
        line = line;
    f.close();
    return line;
}
char* ZTools::hex2ascii(int hex,char *ascii,int len)
{
    char* _ascii = ascii;
    while(len--)
        *ascii++ = ((hex >> (len << 2))&0x0f) > 9?((hex >> (len << 2))&0x0f) - 10 +'A':((hex >> (len << 2))&0x0f) + '0';
    return _ascii;

}

//    int h;
//    while(len--)
//        ascii++ = (h = ((hex >> (len << 2))&0x0f)) > 9?h - 10 +'a':h + '0';
quint64 ZTools::ascii2hex(const char *ascii,int len)
{
    quint64 hex = 0;
    while(len--)
        hex |=  isdigit(*ascii)?quint64(*ascii++ -'0')<<(len<<2):isalpha(*ascii)?quint64(tolower(*ascii++)-'a'+10)<<(len<<2):0<<(len<<2);
    return hex;
}
void ZTools::msleep(int msecs)
{
    usleep(msecs * 1000);
}
int ZTools::getCarID_bk()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach(QHostAddress address,list)
    {
        QString ip = address.toString();
        if(ip.startsWith("192.168."))
        {
            return ip.split(QChar('.'))[2].toInt();
        }
    }
    return 0;
}

static QMutex carIDMutex;
void ZTools::setCarID(int carID)
{
    char buff[100];
    sprintf(buff,"echo 192.168.%d.10 > /etc/myip.txt",carID);
    carIDMutex.lock();
    system(buff);
    carIDMutex.unlock();
}
int ZTools::getCarID()
{
    carIDMutex.lock();
    QString ip = readLineFromFile("/etc/myip.txt",1);
    carIDMutex.unlock();
    if(ip.startsWith("192.168."))
    {
        return ip.split(QChar('.'))[2].toInt();
    }
    else
        return 0;
}
void ZTools::singleShot(int msecs,void (*member)())
{
    QTimer::singleShot(msecs,new ZTools(member),SLOT(Proxy()));
}

void ZTools::Proxy()
{
    _member();
    delete this;
}


void ZTools::singleShot(int msecs,void (*member)(void*),void *data)
{
    QTimer::singleShot(msecs,new ZTools(member,data),SLOT(ProxyWithPara()));
}
void ZTools::ProxyWithPara()
{
    _member_with_para(_member_para);
    delete this;
}
QByteArray ZTools::str2unicode(QString str)
{
    QByteArray bytes;
//    qDebug("111111111111111");
//    int i = 0;
//    foreach(const QChar& qChar,str)
//    {
//        qDebug("%d : %x",i,qChar.unicode());
//        i++;
//    }

//    static QTextCodec* codec = QTextCodec::codecForName("UTF-8");
//    str = codec->toUnicode(str.toAscii().data());
//    i =0;
//    qDebug("2222222222222");
    foreach(const QChar& qChar,str)
    {
//        qDebug("%d : %x",i,qChar.unicode());
        bytes.append(qChar.unicode()&0xff);
        bytes.append(qChar.unicode()>>8&0xff);
//        i++;
    }
//    qDebug("33333333333333");
//        for(int i =0;i<bytes.length();i++)
//        {
//            qDebug("%d : %x",i,bytes.data()[i]);
//        }
    return bytes;
}
//QByteArray ZTools::str2unicode(QString str)
//{
//    QByteArray bytes;
//    static QTextCodec* codec = QTextCodec::codecForName("UTF-8");
//    str = codec->toUnicode(str.toAscii().data());
//    int i =0;
//        foreach(const QChar& qChar,str)
//        {
//            qDebug("%d : %x",i,str[i].unicode());
//            bytes.append(qChar.unicode()>>8&0xff);
//            bytes.append(qChar.unicode()&0xff);
//            i++;
//        }
//    qDebug("xxxxxxxxxxxxxxxxxxxxxx");
//    for(int i =0;i<bytes.length();i++)
//    {
//        qDebug("%d : %x",i,bytes.data()[i]);
//    }
//    return bytes;
//}
