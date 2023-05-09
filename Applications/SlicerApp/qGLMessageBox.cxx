#include "qGLMessageBox.h"

void GLMessageBoxStyle::setMessage(const QString message, const QPoint * pPoint)
{
  if (message.isEmpty())
    return;

  QFontMetrics fm1(font());
  setFixedSize(fm1.width(message) + 30, 30);

  parent = parentWidget(); 

  if (width() > parent->width() && parent->width() > 80) //���ڸ���
  {
    setFixedSize(parent->width() - 60, 60);
    setWordWrap(true); //���в���
  }
  else
  {
    setWordWrap(false);
  }

  setText(message);

  if (nullptr != parent)
  {
    if (nullptr != pPoint) 
    {
      move(parent->mapFromGlobal(*pPoint));
      parent = nullptr;
    }
    else //���û�и�����������ʾ�����м�
    {
      move((parent->width() - width()) >> 1, (parent->height() - height()) >> 1);
    }
  }

  setVisible(true);
  transparent = 200; //͸���ȴ�200��ʼ

  timer->start(30);
}

GLMessageBoxStyle::GLMessageBoxStyle()
  :parent(nullptr), enter(false), enableEvent(false), transparent(200)
{
  setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  setAlignment(Qt::AlignCenter);

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &GLMessageBoxStyle::onTimer);
}

GLMessageBoxStyle::~GLMessageBoxStyle()
{
  deleteLater();
}

GLMessageBoxStyle &GLMessageBoxStyle::instance()
{
  static GLMessageBoxStyle msg;
  return msg;
}

//ʹ��ʾ�򽥽�͸����ֱ����ʧ
void GLMessageBoxStyle::onTimer()
{
  if (enter) //���ŵ���Ϣ��ʱ������仯
    return;

  transparent -= 3;
  if (transparent > 0)
  {
    if (parent && parentWidget()) //������ڸ��࣬����С�����ڸ�����м���С��2������С�������򲻻��С
    {
      QPoint pt((parentWidget()->width() - width()) >> 1, (parentWidget()->height() - height()) >> 1);
      if (pos() != pt)
        move(pt);
    }
    //��ɫ����-3
    setStyleSheet(QString::fromUtf8("color: white; font: 12px \"Microsoft YaHei\"; border-radius: 5px; background-color:rgba(80, 80, 80, %1);").arg(transparent));
  }
  else
  {
    timer->stop();
    setVisible(false);
  }
}

void GLMessageBoxStyle::enterEvent(QEvent *e)
{
  //�����֮��������ʾ
  if (enableEvent)
  {
    enter = true;
    transparent = 200;
    setStyleSheet(QString("color:white;font:12px \"Microsoft YaHei\";border-radius:5px;background-color:rgba(80, 80, 80, %1);").arg(transparent));
  }

  QLabel::enterEvent(e);
}

void GLMessageBoxStyle::leaveEvent(QEvent *e)
{
  if (enableEvent)
  {
    enter = false;
  }
  QLabel::leaveEvent(e);
}

GLMessageBox::GLMessageBox(QString title, QString txt, QVector<QPair<QAbstractButton*, QMessageBox::ButtonRole>> btnVec, QMessageBox::Icon icon, QWidget* parent)
	: QMessageBox(parent), m_width(256), m_height(110)
{
	setWindowTitle(title);
	setText(txt);
	for (int i = 0; i < btnVec.size(); i++)
	{
		addButton(btnVec.at(i).first, btnVec.at(i).second);
	}
	setIcon(icon);
	exec();
}

GLMessageBox::GLMessageBox(QWidget* parent) : QMessageBox(parent)
{

}

QAbstractButton* GLMessageBox::result()
{	
	return clickedButton();
}

void GLMessageBox::SetSize(int width, int height)
{
	m_width = width;
	m_height = height;
}

void GLMessageBox::resizeEvent(QResizeEvent * event)
{
	//setFixedSize(m_width, m_height);
}

QMessageBox::ButtonRole GLMessageBox::queRenInfo(QString title, QString txt, QString btnText)
{
  setWindowTitle(title);
  this->setIcon(QMessageBox::Information);
  this->setText(txt);
  QPushButton *btn = new QPushButton(btnText);
  this->addButton(btn, QMessageBox::AcceptRole);
  this->exec();
  return buttonRole(clickedButton());
}

QMessageBox::ButtonRole GLMessageBox::queRenWarn(QString title, QString txt, QString btnText)
{
  setWindowTitle(title);
  this->setIcon(QMessageBox::Warning);
  this->setText(txt);
  QPushButton *btn = new QPushButton(QStringLiteral("ȷ��"));
  this->addButton(btn, QMessageBox::AcceptRole);
  this->exec();
  return buttonRole(clickedButton());
}

QMessageBox::ButtonRole GLMessageBox::queRenError(QString title, QString txt, QString btnText)
{
  setWindowTitle(title);
  this->setIcon(QMessageBox::Critical);
  this->setText(txt);
  QPushButton *btn = new QPushButton(QStringLiteral("ȷ��"));
  this->addButton(btn, QMessageBox::AcceptRole);
  this->exec();
  return buttonRole(clickedButton());
}

QMessageBox::ButtonRole GLMessageBox::YesOrNoInfo(QString title, QString txt, QString txtYes, QString txtNo)
{
  setWindowTitle(title);
  this->setIcon(QMessageBox::Information);
  this->setText(txt);
  QPushButton *btnYes = new QPushButton(txtYes);
  QPushButton *btnNo = new QPushButton(txtNo);
  this->addButton(btnYes, QMessageBox::YesRole);
  this->addButton(btnNo, QMessageBox::NoRole);
  this->exec();
  return buttonRole(clickedButton());
}

QMessageBox::ButtonRole GLMessageBox::YesOrNoWarn(QString title, QString txt, QString txtYes, QString txtNo)
{
  setWindowTitle(title);
  this->setIcon(QMessageBox::Warning);
  this->setText(txt);
  QPushButton *btnYes = new QPushButton(txtYes);
  QPushButton *btnNo = new QPushButton(txtNo);
  this->addButton(btnYes, QMessageBox::YesRole);
  this->addButton(btnNo, QMessageBox::NoRole);
  this->exec();
  return buttonRole(clickedButton());
}

QMessageBox::ButtonRole GLMessageBox::YesOrNoError(QString title, QString txt, QString txtYes, QString txtNo)
{
  setWindowTitle(title);
  this->setIcon(QMessageBox::Critical);
  this->setText(txt);
  QPushButton *btnYes = new QPushButton(txtYes);
  QPushButton *btnNo = new QPushButton(txtNo);
  this->addButton(btnYes, QMessageBox::YesRole);
  this->addButton(btnNo, QMessageBox::NoRole);
  this->exec();
  return buttonRole(clickedButton());
}

GLMessageBox::~GLMessageBox()
{
}
