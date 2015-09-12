// -*- c++ -*-

#ifndef _helpdialog_h
#define _helpdialog_h

#include <QDialog>
#include <QLabel>
#include <QUrl>

#include <string>
#include <vector>

class QPushButton;
class QTextBrowser;
class QLineEdit;
class QTimer;
class QToolButton;

class HelpDialog: public QDialog {
  Q_OBJECT
public:

  struct Info {
    struct Source {
      std::string source;
      std::string name;
      std::string defaultlink;
    };
    std::string path;
    std::vector<Source> src;
  };

  HelpDialog(QWidget* parent, const Info& hdi);

  void setSource(const std::string& source);

  const std::string& helpPath() const
    { return info.path; }

public Q_SLOTS:
  void showdoc(const int doc, const std::string& tag = "");
  void showsource(const std::string& source, const std::string& tag = "");

private Q_SLOTS:
  void searchDocument();
  void printHelp();
  void toggleSearchBar(bool);

private:
  QTextBrowser* tb;
  Info info;

  QPushButton* pushbackward;
  QPushButton* pushforward;
  QPushButton* printbutton;
  QToolButton* startsearchbutton;
  QPushButton* closebutton;
  QWidget* searchBar;
  QLineEdit* searchEdit;
  QTimer* mSearchTimer;
};

#endif
