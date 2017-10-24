#include "baseheader.h"

void showCritical(const QString content){
    QMessageBox::critical(NULL,
            "critical", content,
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::Yes);
}
