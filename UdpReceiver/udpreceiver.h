#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QObject>
#include <QThread>
#include "api_generated.h"
#include <QUdpSocket>

class UdpReceiver : public QThread
{
    Q_OBJECT
public:
    explicit UdpReceiver(QObject *parent = nullptr);
    ~UdpReceiver();

void run() override;


signals:
    void figureReceived(const Figure* figure);

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket* socket;
};

#endif // UDPRECEIVER_H
