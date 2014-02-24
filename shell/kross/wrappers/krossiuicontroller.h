#ifndef KDEVPLATFORM_KROSSIUICONTROLLER_H
#define KDEVPLATFORM_KROSSIUICONTROLLER_H

#include<QtCore/QVariant>

//This is file has been generated by xmltokross, you should not edit this file but the files used to generate it.

namespace KDevelop { class IToolViewFactory; }
namespace KDevelop { class IUiController; }
namespace Handlers
{
	QVariant kDevelopIUiControllerHandler(KDevelop::IUiController* type);
	QVariant kDevelopIUiControllerHandler(const KDevelop::IUiController* type);

	QVariant kDevelopIToolViewFactoryHandler(KDevelop::IToolViewFactory* type);
	QVariant kDevelopIToolViewFactoryHandler(const KDevelop::IToolViewFactory* type);

}

#endif