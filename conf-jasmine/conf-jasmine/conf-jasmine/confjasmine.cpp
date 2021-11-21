#include "confjasmine.h"

#include "linphonec.h"
#include <QOpenGLFunctions>
#include <mediastreamer2/msogl_functions.h>
#include "MSFunctions.hpp"
#include <QOpenGLFramebufferObject>
#include <QQuickFramebufferObject>

#include "CameraPreview.hpp"
#include "Camera.hpp"
#include <QQuickView>
#include <QTimer>
//#include <QQuickWidget>
constexpr char MainQmlUri[] = "Linphone";

template<typename T>
static inline void registerType(const char *name) {
	qmlRegisterType<T>(MainQmlUri, 1, 0, name);
}

void confjasmine_linphonecx_callback(int status, void * date)
{
	confjasmine * p = (confjasmine *)date;



#if 1
	switch (status)
	{
	case 0:
	{
		QCoreApplication::postEvent(p, new QEvent(custom_event_0));
	}
	break;
	case 1:
	{
		QCoreApplication::postEvent(p, new QEvent(custom_event_1));
	}
	break;
	default:
		break;
	}

#endif
	
}






confjasmine::confjasmine(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	registerType<CameraPreview>("CameraPreview");
	registerType<Camera>("Camera");
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(pushButton_slots()));
	//
	linphonecx::linphonecx_start(confjasmine_linphonecx_callback,this);
	while (!linphonecx::linphonecx__status())
	{
		Sleep(1);
	}
	//
	linphonecx::linphonecx_regsiter("1007", "2020", "192.168.0.107");
	while (!linphonecx::linphonecx_regsiter_status())
	{
		Sleep(1);
	}
	//
	ui.quick_widget_remote_video->setSource(QUrl::fromLocalFile("c.qml"));
	//ui.quick_widget_local_video->setSource(QUrl::fromLocalFile("c.qml"));

}

void confjasmine::customEvent(QEvent *e)
{
	if (e->type() == custom_event_0)
	{
		//ui.quick_widget_local_video->setSource(QUrl::fromLocalFile("c.qml"));
		ui.quick_widget_remote_video->setSource(QUrl::fromLocalFile("c.qml"));
	}
	else if (e->type() == custom_event_1)
	{
		//ui.quick_widget_local_video->setSource(QUrl::fromLocalFile("a.qml"));
		ui.quick_widget_remote_video->setSource(QUrl::fromLocalFile("b.qml"));
	}

}



void confjasmine::pushButton_slots()
{

	//linphonecx::linphonecx_launch_call("s");

	//ui.quick_widget_remote_video->setSource(QUrl::fromLocalFile("b.qml"));

	linphonecx::linphonecx_launch_call("s");


	//linphonecx::linphonecx_accept_call();


		//linphone_core_reload_video_devices(linphonecx::get_core());

	//LinphoneVideoDefinition * v;

	//linphone_core_set_preferred_video_definition(linphonecx::get_core(), v);
	//linphone_core_set_preferred_video_definition(linphonecx::get_core(), "vga");
// 	CameraPreview *  p = new CameraPreview();
// 	p->setWidth(100);
// 	p->setHeight(100);

	//QQuickView *view = new QQuickView;
	//view->setResizeMode(QQuickView::SizeRootObjectToView);
	//view->setSource(QUrl(QStringLiteral("qrc:/a.qml")));
	//view->setSource(QUrl::fromLocalFile("a.qml"));

	//QUrl source("qrc:/qml/Mybattery.qml");

	//ui.quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

	//ui.quickWidget->setClearColor(QColor(Qt::transparent));



	//view->show();

	// 	QQuickWidget *view1 = new QQuickWidget;
	// 	view1->setSource(QUrl::fromLocalFile("myqmlfile.qml"));
	// 	view1->show();


		//ui.openglll->context()->se

	// 	//QOpenGLFramebufferObjectFormat format;
	// 	ui.openglll->context()->format().set .setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
	// 	ui.openglll->context()->format().setInternalTextureFormat(GL_RGBA8);
	// 	ui.openglll->context()->format().setSamples(4);

		//ui.openglll->set

		//MSFunctions::getInstance()->bind(ui.openglll->context()->functions());
		//linphonecx::linphonecx_launch_audio_call(MSFunctions::getInstance()->getFunctions(), 0);

		//CoreManager * p = new CoreManager(this,"C:/sdk")
		//CoreManager::init(this, "C:/sdk");
		//auto instance = CoreManager::getInstance();
}

