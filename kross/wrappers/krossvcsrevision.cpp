//This is file has been generated by xmltokross, you should not edit this file but the files used to generate it.

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <kross/core/manager.h>
#include <kross/core/wrapperinterface.h>
#include <vcs/vcsrevision.h>

class KrossKDevelopVcsRevision : public QObject, public Kross::WrapperInterface
{
	Q_OBJECT
	Q_ENUMS(RevisionType)
	Q_FLAGS(RevisionType Special GlobalNumber FileNumber Date Invalid UserType)

	Q_ENUMS(RevisionSpecialType)
	Q_FLAGS(RevisionSpecialType Head Working Base Previous Start UserSpecialType)

	public:
		enum KrossRevisionType { Special=KDevelop::VcsRevision::Special, GlobalNumber=KDevelop::VcsRevision::GlobalNumber, FileNumber=KDevelop::VcsRevision::FileNumber, Date=KDevelop::VcsRevision::Date, Invalid=KDevelop::VcsRevision::Invalid, UserType=KDevelop::VcsRevision::UserType };
		enum KrossRevisionSpecialType { Head=KDevelop::VcsRevision::Head, Working=KDevelop::VcsRevision::Working, Base=KDevelop::VcsRevision::Base, Previous=KDevelop::VcsRevision::Previous, Start=KDevelop::VcsRevision::Start, UserSpecialType=KDevelop::VcsRevision::UserSpecialType };
		KrossKDevelopVcsRevision(KDevelop::VcsRevision* obj, QObject* parent=0) : QObject(parent), wrapped(obj)		{ setObjectName("KDevelop::VcsRevision"); }
		void* wrappedObject() const { return wrapped; }

		Q_SCRIPTABLE KDevelop::VcsRevision operator=(const KDevelop::VcsRevision& x0) { return wrapped->operator=(x0); }
		Q_SCRIPTABLE void setRevisionValue(const QVariant& x0, KDevelop::VcsRevision::RevisionType x1) { wrapped->setRevisionValue(x0, x1); }
		Q_SCRIPTABLE KDevelop::VcsRevision::RevisionType revisionType() const { return wrapped->revisionType(); }
		Q_SCRIPTABLE QVariant revisionValue() const { return wrapped->revisionValue(); }
		Q_SCRIPTABLE QString prettyValue() const { return wrapped->prettyValue(); }
		Q_SCRIPTABLE bool operator==(const KDevelop::VcsRevision& x0) const { return wrapped->operator==(x0); }
		Q_SCRIPTABLE KDevelop::VcsRevision createSpecialRevision(KDevelop::VcsRevision::RevisionSpecialType x0) { return wrapped->createSpecialRevision(x0); }
	private:
		KDevelop::VcsRevision* wrapped;
};

bool krossvcsrevision_registerHandler(const QByteArray& name, Kross::MetaTypeHandler::FunctionPtr* handler)
{ Kross::Manager::self().registerMetaTypeHandler(name, handler); return false; }

namespace Handlers
{
QVariant _kDevelopVcsRevisionHandler(void* type)
{
	if(!type) return QVariant();
	KDevelop::VcsRevision* t=static_cast<KDevelop::VcsRevision*>(type);
	Q_ASSERT(dynamic_cast<KDevelop::VcsRevision*>(t));
	return qVariantFromValue((QObject*) new KrossKDevelopVcsRevision(t, 0));
}
bool b_KDevelopVcsRevision=krossvcsrevision_registerHandler("KDevelop::VcsRevision*", _kDevelopVcsRevisionHandler);
QVariant kDevelopVcsRevisionHandler(KDevelop::VcsRevision* type){ return _kDevelopVcsRevisionHandler(type); }
QVariant kDevelopVcsRevisionHandler(const KDevelop::VcsRevision* type) { return _kDevelopVcsRevisionHandler((void*) type); }

}
#include "krossvcsrevision.moc"
