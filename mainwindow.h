#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QMediaPlayer>
#include <QAudioOutput>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void check_path();
    void on_add_playlist_button_clicked();
    void show_list_playlist_menu(const QPoint &pos);
    void on_list_playlists_itemChanged(QListWidgetItem *item);
    void on_list_playlists_itemSelectionChanged();
    void on_searchbox_textChanged();
    void on_volume_valueChanged(int value);
    void on_volume_icon_clicked();
    void play_music();
    void on_songlist_itemClicked(QListWidgetItem *item);
    void on_main_button_clicked();
    void display_time();
    QString seconds_to_time_format(qint64 total_seconds);
    void on_duration_changed(qint64 duration_ms);
    void on_position_changed(qint64 position_ms);
    void on_timeline_sliderMoved(int position);
    void on_previous_clicked();
    void on_next_clicked();
    void on_playback_speed_valueChanged(int value);
    void on_loop_clicked();
    void on_add_favourite_clicked();
    void on_speed_value_clicked();
    void on_autoplay_clicked();
    void autoplay_songs(QMediaPlayer::MediaStatus status);
    void on_plus_5s_clicked();
    void on_minus_5s_clicked();
    void on_favourties_clicked();
    void on_hide_clicked();
    void on_open_file_clicked();

    void on_clear_search_clicked();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QAudioOutput *audio_output;
};
#endif // MAINWINDOW_H
