#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QSettings>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QDebug>
#include <QFont>
#include <QRandomGenerator>

QStringList fav_palylist;
QStringList supported_extensions = { ".mp3", ".wav", ".webm" };

QString default_folder_path = "";
QString selected_song;
QString selected_song_extension;
QString total_duration_str;
QString old_playlist_name;

QSettings settings("Music Player", "My App");

int default_volume = 30;

bool is_mute = false;
bool playlist_selected = false;
bool started = false;
bool loop_enabled = false;
bool fav_added = false;
bool autoplay_enabled = false;
bool fav_playlist_selected = false;
bool is_hidden = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , player(new QMediaPlayer(this))
    , audio_output(new QAudioOutput(this))
{
    ui->setupUi(this);

    ui->list_playlists->setStyleSheet(
        "QListWidget { border-radius: 10px; color: #d0d1d6; }"
        "QListWidget:focus { outline: none; }"
        "QListWidget::item { border: 0; border-bottom: 1px solid #d0d1d6; padding: 5px; color: #d0d1d6; }"
        "QListWidget::item:selected { background: #313237; color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
        "QListWidget::item:hover { background: #313237; }"
        "QScrollBar:vertical { border: none; background: #181b22; width: 8px; }"
        "QScrollBar::handle:vertical { background: #d0d1d6; min-height: 20px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }"
        "QScrollBar::handle:vertical:hover { background: #e65100; }"
        );

    ui->songlist->setStyleSheet(
        "QListWidget { border-radius: 10px; color: #d0d1d6; }"
        "QListWidget:focus { outline: none; }"
        "QListWidget::item { border: 0; border-bottom: 1px solid #d0d1d6; padding: 5px; color: #d0d1d6; }"
        "QListWidget::item:selected { background: #313237; color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
        "QListWidget::item:hover { background: #313237; }"
        "QScrollBar:vertical { border: none; background: #181b22; width: 8px; }"
        "QScrollBar::handle:vertical { background: #d0d1d6; min-height: 20px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }"
        "QScrollBar::handle:vertical:hover { background: #e65100; }"
        "QScrollBar:horizontal { border: none; background: #181b22; width: 8px; }"
        "QScrollBar::handle:horizontal { background: #d0d1d6; min-height: 20px; }"
        "QScrollBar::add-line:horizontal, QScrollBar::sub-line:vertical, QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }"
        );

    ui->add_playlist_button->setStyleSheet(
        "QPushButton { color: #d0d1d6; border: 0; border-top: 1px solid #d0d1d6; border-radius: 0; border-bottom-left-radius: 10px; border-bottom-right-radius: 10px; }"
        "QPushButton:hover { color: #e65100; }"
        );

    ui->timeline->setStyleSheet(
        "QSlider::groove:horizontal { background: #313237; border-radius: 2px; height: 5px; }"
        "QSlider::handle:horizontal { background: #e65100; margin: -3px 0; width: 10px; height: 10px; border-radius: 5px; }"
        "QSlider::sub-page:horizontal { background: #d0d1d6; border-radius: 2px; }"
        );

    ui->volume->setStyleSheet(
        "QSlider::groove:horizontal { background: #313237; border-radius: 2px; height: 5px; }"
        "QSlider::handle:horizontal { background: #e65100; margin: -3px 0; width: 10px; height: 10px; border-radius: 5px; }"
        "QSlider::sub-page:horizontal { background: #d0d1d6; border-radius: 2px; }"
        );

    ui->playback_speed->setStyleSheet(
        "QSlider::groove:horizontal { background: #313237; border-radius: 2px; height: 5px; }"
        "QSlider::handle:horizontal { background: #e65100; margin: -3px 0; width: 10px; height: 10px; border-radius: 5px; }"
        "QSlider::sub-page:horizontal { background: #d0d1d6; border-radius: 2px; }"
        );

    ui->hide->setStyleSheet(
        "QPushButton { border: 0; border-radius: 10px; background: #181b22; color: #d0d1d6; }"
        "QPushButton:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
        );

    ui->youtube->setStyleSheet(
        "QPushButton { border: 0; border-radius: 10px; background: #181b22; color: #d0d1d6; }"
        "QPushButton:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
        );

    ui->favourties->setStyleSheet(
        "QPushButton { border: 0; border-radius: 10px; background: #181b22; color: #d0d1d6; }"
        "QPushButton:hover { color: #fa3c3c; border: 0; border-bottom: 1px solid #fa3c3c; }"
        );

    ui->open_file->setStyleSheet(
        "QPushButton { border: 0; border-radius: 10px; background: #181b22; color: #d0d1d6; }"
        "QPushButton:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
        );

    ui->settings->setStyleSheet(
        "QPushButton { border: 0; border-radius: 10px; background: #181b22; color: #d0d1d6; }"
        "QPushButton:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
        );

    ui->add_favourite->setStyleSheet(
        "QPushButton { border: 0; border-radius: 10px; }"
        "QPushButton:hover { color: #fa3c3c; border: 0; border-bottom: 1px solid #fa3c3c; }"
        );

    ui->loop->setStyleSheet(
        "QPushButton { border: 0; border-radius: 10px; }"
        "QPushButton:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
        );

    ui->autoplay->setStyleSheet(
        "QPushButton { border: 0; border-radius: 10px; }"
        "QPushButton:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
        );

    ui->plus_5s->setStyleSheet(
        "QPushButton { border: 0; border-radius: 10px; }"
        "QPushButton:hover { color: #e65100; }"
        );

    ui->minus_5s->setStyleSheet(
        "QPushButton { border: 0; border-radius: 10px; }"
        "QPushButton:hover { color: #e65100; }"
        );

    ui->previous->setStyleSheet(
        "QPushButton:hover { color: #e65100; }"
        );

    ui->next->setStyleSheet(
        "QPushButton:hover { color: #e65100; }"
        );

    ui->speed_value->setStyleSheet(
        "QPushButton:hover { color: #e65100; }"
        );

    ui->index_songs->setStyleSheet(
        "QLabel:hover { color: #e65100; }"
        );

    ui->current_time->setStyleSheet(
        "QLabel:hover { color: #e65100; }"
        );

    ui->total_time->setStyleSheet(
        "QLabel:hover { color: #e65100; }"
        );

    ui->clear_search->setStyleSheet(
        "QPushButton:hover { color: #e65100; }"
        );

    ui->clear_search->hide();

    QSettings settings("Music Player", "My App");
    settings.beginGroup("Playlists");

    int count = settings.value("count", 0).toInt();
    for (int i = 0; i < count; ++i) {
        QString playlistName = settings.value(QString::number(i), "Default").toString();
        QListWidgetItem *item = new QListWidgetItem(playlistName);
        ui->list_playlists->addItem(item);

        if (i == 0) {
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        } else {
            item->setFlags(item->flags() | Qt::ItemIsEditable);
        }
    }

    settings.endGroup();

    if (count == 0) {
        QListWidgetItem *default_playlist = new QListWidgetItem("Default");
        ui->list_playlists->addItem(default_playlist);
    }

    default_folder_path = settings.value("default_folder_path", "").toString();

    ui->list_playlists->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->list_playlists, &QListWidget::customContextMenuRequested, this, &MainWindow::show_list_playlist_menu);

    ui->volume->setValue(settings.value("volume", default_volume).toInt());

    if (default_folder_path == "")
    {
        return;
    }
    else
    {
        ui->list_playlists->setCurrentRow(0);
    }

    fav_palylist = settings.value("favs", QVariant()).toStringList();
    loop_enabled = settings.value("loop_enabled").toBool();
    autoplay_enabled = settings.value("autoplay_enabled").toBool();

    if (loop_enabled)
    {
        ui->loop->setStyleSheet(
            "QPushButton { border: 0; border-radius: 10px; color: #e65100; }"
            "QPushButton:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
            );
        player->setLoops(QMediaPlayer::Infinite);

        loop_enabled = true;
        autoplay_enabled = false;

        ui->autoplay->setStyleSheet(
            "QPushButton { border: 0; border-radius: 10px; }"
            "QPushButton:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
            );
        ui->autoplay->setText("");
        disconnect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::autoplay_songs);
    }

    if (autoplay_enabled)
    {
        ui->autoplay->setStyleSheet(
            "QPushButton { border: 0; border-radius: 10px; color: #e65100; }"
            "QPushButton:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
            );
        ui->autoplay->setText("");
        connect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::autoplay_songs);

        autoplay_enabled = true;
        loop_enabled = false;

        ui->loop->setStyleSheet(
            "QPushButton { border: 0; border-radius: 10px; }"
            "QPushButton:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
            );
        player->setLoops(QMediaPlayer::Once);
    }
}

MainWindow::~MainWindow()
{
    settings.beginGroup("Playlists");
    settings.remove("");
    settings.endGroup();

    settings.beginGroup("Playlists");
    settings.setValue("count", ui->list_playlists->count());
    for (int i = 0; i < ui->list_playlists->count(); i++)
    {
        QListWidgetItem *item = ui->list_playlists->item(i);
        settings.setValue(QString::number(i), item->text());
    }
    settings.endGroup();

    settings.setValue("default_folder_path", default_folder_path);
    settings.setValue("volume", ui->volume->value());
    settings.setValue("favs", fav_palylist);
    settings.setValue("loop_enabled", loop_enabled);
    settings.setValue("autoplay_enabled", autoplay_enabled);

    delete ui;
}

void MainWindow::check_path()
{
    for (const QString &ext : supported_extensions)
    {
        QString potential_path = default_folder_path + "/" + selected_song + ext;
        if (QFile::exists(potential_path))
        {
            selected_song_extension = potential_path;
            break;
        }
    }
}

void MainWindow::on_add_playlist_button_clicked()
{
    QListWidgetItem *new_item = new QListWidgetItem("New Playlist");
    ui->list_playlists->addItem(new_item);
    ui->list_playlists->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    new_item->setFlags(new_item->flags() | Qt::ItemIsEditable);
    ui->list_playlists->editItem(new_item);
}

void MainWindow::show_list_playlist_menu(const QPoint &pos)
{
    QListWidgetItem *item = ui->list_playlists->itemAt(pos);
    if(!item)
    {
        return;
    }

    QMenu list_playlist_menu(this);

    list_playlist_menu.setStyleSheet(
        "QMenu { background: #0b0e15; color: #d0d1d6; }"
        "QMenu::item { border: 0; border-bottom: 1px solid #d0d1d6; padding: 5px 40px; }"
        "QMenu::item:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
        "QMenu::item:selected { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
        );

    QAction *rename_playlist = new QAction("Rename", this);
    QAction *delete_playlist = new QAction("Delete", this);

    connect(rename_playlist, &QAction::triggered, [=]() {
        if(ui->list_playlists->row(item)== 0)
        {
            QMessageBox::warning(this, "Deafult Playlist Rename", "Sorry! You Cannot Rename the Default Playlist", QMessageBox::Ok);
        } else
        {
            ui->list_playlists->editItem(item);
        }
    });
    connect(delete_playlist, &QAction::triggered, [=]() {
        if(ui->list_playlists->row(item)== 0)
        {
            QMessageBox::warning(this, "Deafult Playlist Deletion", "Sorry! You Cannot Delete the Default Playlist", QMessageBox::Ok);
        } else
        {
            delete item;
        }
    });

    list_playlist_menu.addAction(rename_playlist);
    list_playlist_menu.addAction(delete_playlist);

    list_playlist_menu.exec(ui->list_playlists->mapToGlobal(pos));
}

void MainWindow::on_list_playlists_itemChanged(QListWidgetItem *item)
{
    QString new_name = item->text().trimmed();
    if (new_name.isEmpty())
    {
        QMessageBox::warning(this, "Invalid Name", "Playlist name cannot be empty.", QMessageBox::Ok);
        delete item;
        return;
    }

    for (int i = 0; i < ui->list_playlists->count(); ++i)
    {
        QListWidgetItem *otherItem = ui->list_playlists->item(i);
        if (otherItem != item && otherItem->text() == new_name)
        {
            QMessageBox::warning(this, "Duplicate Name", "A playlist with this name already exists.", QMessageBox::Ok);
            delete item;
            return;
        }
    }
}


void MainWindow::on_list_playlists_itemSelectionChanged()
{
    if(ui->list_playlists->currentRow() == 0 && default_folder_path == "")
    {
        QMessageBox select_folder(this);
        select_folder.setWindowTitle("Select Folder");
        select_folder.setText("Please select a Folder.");

        QPushButton *browse_button = select_folder.addButton("Choose Folder", QMessageBox::ActionRole);
        select_folder.addButton(QMessageBox::Cancel);

        select_folder.exec();

        if (select_folder.clickedButton() == browse_button)
        {
            QString choosen_folder_path = QFileDialog::getExistingDirectory(this, "Select Folder", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

            if (!choosen_folder_path.isEmpty())
            {
                default_folder_path = choosen_folder_path;
            }
            else
            {
                QMessageBox::warning(this, "No Folder Selected", "You did not select any folder.");
            }
            QDir dir(default_folder_path);
            QStringList filters;
            filters << "*mp3" << "*wav" << "*webm";

            QFileInfoList song_list = dir.entryInfoList(filters, QDir::Files);

            ui->songlist->clear();

            foreach (const QFileInfo &fileInfo, song_list)
            {
                ui->songlist->addItem(fileInfo.baseName());
            }

            if(ui->list_playlists->currentRow() == 0)
            {
                ui->playlist_name->setText(ui->list_playlists->currentItem()->text() + " - " + default_folder_path);
            }
            else
            {
                ui->playlist_name->setText(ui->list_playlists->currentItem()->text());
            }

            ui->index_songs->setText(QString::number(ui->songlist->count()) + " items");

            playlist_selected = true;
        }
    }
    else if (ui->list_playlists->currentRow() == 0)
    {
        QDir dir(default_folder_path);
        QStringList filters;
        filters << "*mp3" << "*wav" << "*webm";

        QFileInfoList song_list = dir.entryInfoList(filters, QDir::Files);

        ui->songlist->clear();
        foreach (const QFileInfo &fileInfo, song_list)
        {
            ui->songlist->addItem(fileInfo.baseName());
        }

        ui->playlist_name->setText(ui->list_playlists->currentItem()->text() + " - " + default_folder_path);
        ui->index_songs->setText(QString::number(ui->songlist->count()) + " items");

        playlist_selected = true;
    }
    else
    {
        return;
    }
}

void MainWindow::on_searchbox_textChanged()
{
    QString search_text = ui->searchbox->text();

    if (search_text.isEmpty()){
        ui->clear_search->hide();
    }
    else
    {
        ui->clear_search->show();
    }

    if(fav_playlist_selected){
        ui->songlist->clear();
        foreach (const QString &fav_song, fav_palylist) {
            if (fav_song.contains(search_text, Qt::CaseInsensitive)) {
                ui->songlist->addItem(fav_song);
            }
        }
    }
    else if (playlist_selected)
    {
        ui->songlist->clear();
        QDir dir(default_folder_path);
        QStringList filters;
        filters << "*mp3" << "*wav" << "*webm";
        QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files);

        foreach (const QFileInfo &fileInfo, fileList)
        {
            if (fileInfo.fileName().contains(search_text, Qt::CaseInsensitive)) {
                ui->songlist->addItem(fileInfo.baseName());
            }
        }

        ui->index_songs->setText(QString::number(ui->songlist->count()) + " items");
    }
    else
    {
        return;
    }
}

void MainWindow::on_volume_valueChanged(int value)
{
    audio_output->setVolume(value / 100.0);

    if (value == 0)
    {
        ui->volume_icon->setText("");
        ui->volume_icon->setStyleSheet(
            "QPushButton { color: #d0d1d6; }"
            );
    }
    else if (value > 50)
    {
        ui->volume_icon->setText("");
        ui->volume_icon->setStyleSheet(
            "QPushButton { color: #e65100; }"
            );
    }
    else
    {
        ui->volume_icon->setText("");
        ui->volume_icon->setStyleSheet(
            "QPushButton { color: #d0d1d6; }"
            );
    }

    ui->volume_value->setText(QString::number(value) + "%");
}

void MainWindow::on_volume_icon_clicked()
{
    if(!is_mute)
    {
        ui->volume_icon->setText("");
        audio_output->setVolume(0);
        is_mute = true;
    }
    else
    {
        on_volume_valueChanged(ui->volume->value());
        float user_volume = ui->volume->value() / 100.0;
        audio_output->setVolume(user_volume);
        is_mute = false;
    }
}

void MainWindow::play_music()
{
    if (player->playbackState() == QMediaPlayer::PlayingState)
    {
        player->stop();
    }

    player->setSource(QUrl::fromLocalFile(selected_song_extension));
    player->setAudioOutput(audio_output);
    float user_volume = ui->volume->value();
    audio_output->setVolume(user_volume / 100);
    player->play();

    ui->main_button->setText("");
    ui->song_title->setText(selected_song);
    int index_playing = ui->songlist->currentRow() + 1;
    ui->index_songs->setText("Playing " + QString::number(index_playing) + " of " + QString::number(ui->songlist->count()) );

    display_time();
}

void MainWindow::on_songlist_itemClicked(QListWidgetItem *item)
{
    selected_song = item->text();

    check_path();

    if (fav_palylist.contains(selected_song)) {
        fav_added = true;
        ui->add_favourite->setStyleSheet(
            "QPushButton { border: 0; border-radius: 10px; color: #fa3c3c; }"
            "QPushButton:hover { color: #fa3c3c; border: 0; border-bottom: 1px solid #fa3c3c; }"
            );
        ui->add_favourite->setText("");
        QFont font_awesome_solid("Font Awesome 6 Free Solid", 20);
        ui->add_favourite->setFont(font_awesome_solid);

    } else {
        fav_added = false;
        ui->add_favourite->setStyleSheet(
            "QPushButton { border: 0; border-radius: 10px; }"
            "QPushButton:hover { color: #fa3c3c; border: 0; border-bottom: 1px solid #fa3c3c; }"
            );
        ui->add_favourite->setText("");
        QFont font_awesome_regular("Font Awesome 6 Free Regular", 20);
        ui->add_favourite->setFont(font_awesome_regular);
    }

    started = true;
    play_music();

    ui->add_favourite->setEnabled(true);
    ui->autoplay->setEnabled(true);
    ui->next->setEnabled(true);
    ui->previous->setEnabled(true);
}

void MainWindow::on_main_button_clicked()
{
    if (playlist_selected)
    {
        if (!started)
        {
            ui->songlist->setCurrentRow(0);
            selected_song = ui->songlist->currentItem()->text();

            check_path();

            started = true;
            play_music();
        }
        else
        {
            if(player->playbackState() == QMediaPlayer::PlayingState)
            {
                player->pause();
                ui->main_button->setText("");
            }
            else
            {
                player->play();
                ui->main_button->setText("");
            }
        }
    }
    else
    {
        return;
    }
}

void MainWindow::display_time()
{
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::on_duration_changed);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::on_position_changed);
}

QString MainWindow::seconds_to_time_format(qint64 total_seconds)
{
    qint64 minutes = total_seconds / 60;
    qint64 seconds = total_seconds % 60;

    QString formatted_time = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));

    return formatted_time;
}

void MainWindow::on_duration_changed(qint64 duration_ms)
{
    float playback_rate = player->playbackRate();
    qint64 adjusted_duration_ms = duration_ms / playback_rate;
    qint64 total_duration_s = adjusted_duration_ms / 1000;
    total_duration_str = seconds_to_time_format(total_duration_s);

    ui->timeline->setRange(0, duration_ms);
}

void MainWindow::on_position_changed(qint64 position_ms)
{
    float playback_rate = player->playbackRate();

    qint64 adjusted_position_ms = position_ms / playback_rate;
    qint64 position_s = adjusted_position_ms / 1000;
    QString played_time_str = seconds_to_time_format(position_s);

    ui->timeline->setValue(position_ms);
    ui->current_time->setText(played_time_str);
    ui->total_time->setText(total_duration_str);
}

void MainWindow::on_timeline_sliderMoved(int position)
{
    player->pause();
    player->setPosition(position);
    player->play();
}

void MainWindow::on_previous_clicked()
{
    if (ui->songlist->count() == 0)
    {
        return;
    }

    int current_index = ui->songlist->currentRow();
    int prev_index = (current_index - 1 + ui->songlist->count()) % ui->songlist->count();
    ui->songlist->setCurrentRow(prev_index);

    selected_song = ui->songlist->currentItem()->text();
    check_path();

    play_music();
}


void MainWindow::on_next_clicked()
{
    if (ui->songlist->count() == 0)
    {
        return;
    }

    int current_index = ui->songlist->currentRow();
    int next_index = (current_index + 1) % ui->songlist->count();
    ui->songlist->setCurrentRow(next_index);

    selected_song = ui->songlist->currentItem()->text();
    check_path();

    play_music();
}


void MainWindow::on_playback_speed_valueChanged(int value)
{
    ui->speed_value->setText(QString::number(value / 100.0) + "x");
    player->setPlaybackRate(value / 100.0);
    on_duration_changed(player->duration());
}

void MainWindow::on_loop_clicked()
{
    if(loop_enabled)
    {
        ui->loop->setStyleSheet(
            "QPushButton { border: 0; border-radius: 10px; }"
            "QPushButton:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
            );
        player->setLoops(QMediaPlayer::Once);

        loop_enabled = false;
    }
    else
    {
        ui->loop->setStyleSheet(
            "QPushButton { border: 0; border-radius: 10px; color: #e65100; }"
            "QPushButton:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
            );
        player->setLoops(QMediaPlayer::Infinite);

        loop_enabled = true;
        autoplay_enabled = false;

        ui->autoplay->setStyleSheet(
            "QPushButton { border: 0; border-radius: 10px; }"
            "QPushButton:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
            );
        ui->autoplay->setText("");
        disconnect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::autoplay_songs);
    }
}


void MainWindow::on_add_favourite_clicked()
{
    if(fav_added)
    {
        ui->add_favourite->setStyleSheet(
            "QPushButton { border: 0; border-radius: 10px; }"
            "QPushButton:hover { color: #fa3c3c; border: 0; border-bottom: 1px solid #fa3c3c; }"
            );
        ui->add_favourite->setText("");
        QFont font_awesome_regular("Font Awesome 6 Free Regular", 20);
        ui->add_favourite->setFont(font_awesome_regular);

        if(ui->songlist->currentItem() != nullptr)
        {
            fav_palylist.removeAll(selected_song);
        }

        ui->songlist->clear();
        ui->songlist->addItems(fav_palylist);

        fav_added = false;
    }
    else
    {
        ui->add_favourite->setStyleSheet(
            "QPushButton { border: 0; border-radius: 10px; color: #fa3c3c; }"
            "QPushButton:hover { color: #fa3c3c; border: 0; border-bottom: 1px solid #fa3c3c; }"
            );
        ui->add_favourite->setText("");
        QFont font_awesome_solid("Font Awesome 6 Free Solid", 20);
        ui->add_favourite->setFont(font_awesome_solid);

        if(ui->songlist->currentItem() != nullptr){
            fav_palylist.append(ui->songlist->currentItem()->text());
        }

        fav_added = true;
    }
}

void MainWindow::on_speed_value_clicked()
{
    if (player->playbackRate() != 1.0){
        ui->playback_speed->setValue(100);
        on_playback_speed_valueChanged(100);
    }
}

void MainWindow::on_autoplay_clicked()
{
    if(autoplay_enabled)
    {
        ui->autoplay->setStyleSheet(
            "QPushButton { border: 0; border-radius: 10px; }"
            "QPushButton:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
            );
        ui->autoplay->setText("");
        disconnect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::autoplay_songs);

        autoplay_enabled = false;
    }
    else
    {
        ui->autoplay->setStyleSheet(
            "QPushButton { border: 0; border-radius: 10px; color: #e65100; }"
            "QPushButton:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
            );
        ui->autoplay->setText("");
        connect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::autoplay_songs);

        autoplay_enabled = true;
        loop_enabled = false;

        ui->loop->setStyleSheet(
            "QPushButton { border: 0; border-radius: 10px; }"
            "QPushButton:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
            );
        player->setLoops(QMediaPlayer::Once);
    }
}

void MainWindow::autoplay_songs(QMediaPlayer::MediaStatus status){
    if (status == QMediaPlayer::EndOfMedia)
    {
        on_next_clicked();
    }
}

void MainWindow::on_plus_5s_clicked()
{
    player->setPosition(player->position() + 5000);
}


void MainWindow::on_minus_5s_clicked()
{
    player->setPosition(player->position() - 5000);
}


void MainWindow::on_favourties_clicked()
{
    if (fav_playlist_selected)
    {
        ui->favourties->setStyleSheet(
            "QPushButton { border: 0; border-radius: 10px; background: #181b22; color: #d0d1d6; }"
            "QPushButton:hover { color: #fa3c3c; border: 0; border-bottom: 1px solid #fa3c3c; }"
            );

        ui->list_playlists->clearSelection();
        ui->list_playlists->setCurrentRow(0);

        ui->index_songs->setText(QString::number(ui->songlist->count()) + " items");

        fav_playlist_selected = false;
    }
    else
    {
        ui->favourties->setStyleSheet(
            "QPushButton { border: 0; border-radius: 10px; background: #181b22; color: #fa3c3c; }"
            "QPushButton:hover { color: #fa3c3c; border: 0; border-bottom: 1px solid #fa3c3c; }"
            );

        ui->songlist->clear();
        ui->songlist->addItems(fav_palylist);

        ui->playlist_name->setText("Favourites");
        ui->index_songs->setText(QString::number(ui->songlist->count()) + " items");

        fav_playlist_selected = true;
    }
}


void MainWindow::on_hide_clicked()
{
    if (is_hidden)
    {
        ui->hide->setStyleSheet(
            "QPushButton { border: 0; border-radius: 10px; background: #181b22; color: #d0d1d6; }"
            "QPushButton:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
            );

        is_hidden = false;
    }
    else
    {
        ui->hide->setStyleSheet(
            "QPushButton { border: 0; border-radius: 10px; background: #181b22; color: #e65100; }"
            "QPushButton:hover { color: #e65100; border: 0; border-bottom: 1px solid #e65100; }"
            );

        is_hidden = true;
    }
}


void MainWindow::on_open_file_clicked()
{
    QString choosen_song = QFileDialog::getOpenFileName(this, "Select an Audio File", "","Audio Files(*.mp3 *.wav *.webm)");
    if (choosen_song.isEmpty())
    {
        return;
    }

    QFileInfo file(choosen_song);

    player->setSource(QUrl::fromLocalFile(file.absoluteFilePath()));
    player->setAudioOutput(audio_output);
    float user_volume = ui->volume->value();
    audio_output->setVolume(user_volume / 100);
    player->play();

    ui->main_button->setText("");
    ui->song_title->setText(file.baseName());
    ui->index_songs->setText("Playing a Local File");
    display_time();

    started = true;

    ui->add_favourite->setEnabled(false);
    ui->autoplay->setEnabled(false);
    ui->next->setEnabled(false);
    ui->previous->setEnabled(false);
}

void MainWindow::on_clear_search_clicked()
{
    ui->searchbox->clear();
}
