#include "udpreceiver.h"

UdpReceiver::UdpReceiver(QObject *parent) : QThread(parent), socket(nullptr)
{
    // socket->bind(QHostAddress::LocalHost, 10002);
    // connect(socket, &QUdpSocket::readyRead, this, &UdpReceiver::processPendingDatagrams);
}


void UdpReceiver::run()
{
    socket = new QUdpSocket();
    socket->bind(QHostAddress::LocalHost, 10002);
    connect(socket, &QUdpSocket::readyRead, this, &UdpReceiver::processPendingDatagrams, Qt::DirectConnection);
    exec();
}


void UdpReceiver::processPendingDatagrams()
{
    while (socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());

        socket->readDatagram(datagram.data(), datagram.size());
        const Figure* figure = flatbuffers::GetRoot<Figure>(datagram.data());
        emit figureReceived(figure);
    }
}

UdpReceiver::~UdpReceiver()
{
    if (socket) {
        quit();
        wait();
        socket->close();
        delete socket;
    }
}
