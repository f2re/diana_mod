#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "qtHelpDialog.h"

#include <QAction>
#include <QDesktopServices>
#include <QDir>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include <QPrintDialog>
#include <QPrinter>
#include <QTextBrowser>
#include <QTimer>
#include <QToolButton>
#include <QPushButton>
#include <QVBoxLayout>


#include "tb_close.xpm"
#include "tb_right_arrow.xpm"
#include "tb_left_arrow.xpm"
#include "tb_print.xpm"
#include "tb_search.xpm"

HelpDialog::HelpDialog(QWidget* parent, const Info& hdi)
  : QDialog(parent)
  , info(hdi)
{
  setWindowTitle(tr("Help"));

  tb = new QTextBrowser(this);
  tb->setOpenExternalLinks(true);

  std::string source;
  if (not info.src.empty())
    source = info.src[0].source;
  setSource(source);

  pushbackward= new QPushButton(QPixmap(tb_left_arrow_xpm), tr("Back"), this);
  pushbackward->setAutoDefault(false);
  pushbackward->setEnabled(tb->isBackwardAvailable());

  pushforward= new QPushButton(QPixmap(tb_right_arrow_xpm), tr("Forward"), this);
  pushforward->setAutoDefault(false);
  pushforward->setEnabled(tb->isForwardAvailable());

  closebutton= new QPushButton(QPixmap(tb_close_xpm), tr("Close"), this);
  closebutton->setAutoDefault(false);

  printbutton= new QPushButton(QPixmap(tb_print_xpm), tr("Print..."), this);
  printbutton->setAutoDefault(false);

  QAction *searchAction = new QAction(QPixmap(tb_search), tr("&Search..."), this);
  searchAction->setShortcut(QKeySequence::Find);
  searchAction->setCheckable(true);
  addAction(searchAction);

  startsearchbutton = new QToolButton(this);
  startsearchbutton->setDefaultAction(searchAction);
  startsearchbutton->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum, QSizePolicy::ToolButton));

  searchBar = new QWidget();
  QToolButton* searchClearButton = new QToolButton();
  searchClearButton->setIcon(QPixmap(tb_close_xpm));
  searchEdit = new QLineEdit();
  QPushButton* searchButton = new QPushButton(QPixmap(tb_search), tr("Find"));
  searchButton->setAutoDefault(false);

  QHBoxLayout* searchLayout = new QHBoxLayout(this);
  searchLayout->addWidget(searchClearButton);
  searchLayout->addWidget(searchEdit);
  searchLayout->addWidget(searchButton);
  searchLayout->setContentsMargins(0, 0, 0, 0);
  searchBar->setLayout(searchLayout);
  searchBar->hide();

  mSearchTimer = new QTimer(this);
  mSearchTimer->setInterval(500/*ms*/);
  mSearchTimer->setSingleShot(true);

  QGridLayout* layout = new QGridLayout(this);
  setLayout(layout);
  layout->setContentsMargins(2, 2, 2, 2);
  layout->addWidget(pushbackward, 0, 0);
  layout->addWidget(pushforward, 0, 1);
  layout->addWidget(printbutton, 0, 2);
  layout->addWidget(startsearchbutton, 0, 3);
  layout->setColumnStretch(4, 1);
  layout->addWidget(closebutton, 0, 5);
  layout->addWidget(searchBar, 1, 0, 1, 6);
  layout->addWidget(tb, 2, 0, 1, 6);

  resize(800, 600);

  connect(searchClearButton, SIGNAL(clicked()), searchEdit, SLOT(clear()));
  connect(searchEdit, SIGNAL(returnPressed()), searchButton, SLOT(animateClick()));
  connect(searchEdit, SIGNAL(textEdited(const QString&)), mSearchTimer, SLOT(start()));
  connect(searchButton, SIGNAL(clicked()), this, SLOT(searchDocument()));
  connect(mSearchTimer, SIGNAL(timeout()), this, SLOT(searchDocument()));

  connect(pushbackward, SIGNAL(clicked()), tb, SLOT(backward()));
  connect(tb, SIGNAL(backwardAvailable(bool)), pushbackward, SLOT(setEnabled(bool)));
  connect(pushforward, SIGNAL( clicked()), tb, SLOT( forward()));
  connect(tb, SIGNAL(forwardAvailable(bool)), pushforward, SLOT(setEnabled(bool)));
  connect(printbutton, SIGNAL(clicked()), this, SLOT(printHelp()));
  connect(closebutton, SIGNAL(clicked()), this, SLOT( hide()));
  connect(searchAction, SIGNAL(toggled(bool)), this, SLOT(toggleSearchBar(bool)));
}

void HelpDialog::printHelp()
{
  QPrinter printer;
#ifdef linux
  printer.setPrintProgram( QString("lpr") );
#else
  printer.setPrintProgram( QString("lp") );
#endif
  printer.setFullPage(true);

  QPrintDialog *dialog = new QPrintDialog(&printer, this);
  dialog->setWindowTitle(tr("Print Document"));
  if (dialog->exec() != QDialog::Accepted)
    return;
  tb->document()->print(&printer);
}


void HelpDialog::setSource(const std::string& source)
{
  QDir dir(QString::fromStdString(helpPath()));
  QStringList paths(dir.absolutePath());
  tb->setSearchPaths(paths);
  tb->setSource(QString::fromStdString(source));
}


void HelpDialog::showsource(const std::string& source, const std::string& tag)
{
  setSource(source);

  if (not tag.empty())
    tb->scrollToAnchor(QString::fromStdString(tag));

  show();
}


void HelpDialog::showdoc(const int doc, const std::string& tag)
{
  if (doc >= (int)info.src.size())
    return;

  setWindowTitle(QString::fromStdString(info.src[doc].name));
  setSource(info.src[doc].source);

  if (not tag.empty())
    tb->scrollToAnchor(QString::fromStdString(tag));
  else
    tb->scrollToAnchor(QString::fromStdString(info.src[doc].defaultlink));

  show();
}


void HelpDialog::toggleSearchBar(bool on)
{
  searchBar->setVisible(on);
  if (on)
    searchEdit->setFocus(Qt::OtherFocusReason);
}


void HelpDialog::searchDocument()
{
  mSearchTimer->stop();

  const QString search = searchEdit->text();
  if (search.isEmpty())
    return;
  
  if (not tb->find(search)) {
    // wrap around
    QTextCursor tc = tb->textCursor();
    tc.setPosition(0);
    tb->setTextCursor(tc);

    tb->find(search);
  }
}
