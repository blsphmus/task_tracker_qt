#include "decklist.h"
#include "ui_decklist.h"
#include "UI\w_createdeck\createdeck.h"
#include "UI\w_deck\deck.h"
#include "Database/database.h"
#include "QMessageBox"


Deck *DeckList::WindowDeck;

DeckList::DeckList(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DeckList)
{

    WindowDeck = new class::Deck();
    ui->setupUi(this);

    connect(WindowDeck, &Deck::deck_list, this, &DeckList::show);

    UpdateBoards();
}


DeckList::~DeckList()
{
    delete ui;
}


void DeckList::on_btn_Open_pressed()
{
    // сделать проверку в дб на приватность else выдать ошибку
    QListWidgetItem curr_item;
    curr_item = *ui->listWidget->takeItem(ui->listWidget->currentRow()); // чет вернули хз что
    curr_item.text(); //  string name of current item (надо как то его дать в некст окно)


}

void DeckList::UpdateBoards() {
    ui->listWidget->clear();
    for (auto Board : Database::GetParsedBoards()) {

        QString OwnersStr;
        for (auto Owner : Board["Owners"]) {
            OwnersStr += Owner.get<std::string>() + ", ";
        }
        OwnersStr.chop(2);
        QString Text = (Board["Name"].dump() + ", " + Board["Type"].get<std::string>() + ", Owners: ").c_str();
        Text += OwnersStr;
        QListWidgetItem *test_item = new QListWidgetItem;
        test_item->setText(Text);
        ui->listWidget->insertItem(0, test_item);
    }
}

void DeckList::on_btn_Create_pressed()
{
    CreateDeck window;
    window.setModal(true);
    window.exec(); // open create deck window
}



void DeckList::on_btn_Delete_pressed()
{
    if (ui->listWidget->count() == 0) {
        QMessageBox::warning(this, "No boards left", "You have no boards, try to create some or ask somebody for access");
    } else {
        QString Text = ui->listWidget->currentItem()->text().split(" ").at(0);
        Text.chop(2);
        Text.remove(0, 1);
        std::cout << Text.toStdString();
        Database::DeleteBoard(Text);
        ui->listWidget->takeItem(ui->listWidget->currentRow()); // delete current item
    }
}



void DeckList::on_btn_Exit_pressed()
{
    close();
    emit login_window(); // back to loginwindow
}

