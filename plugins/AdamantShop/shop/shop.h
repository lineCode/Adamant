#ifndef SHOP_H
#define SHOP_H

#include <functional>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaProperty>
#include <QObject>
#include <QDebug>
#include <QJsonArray>
#include <shoptemplate.h>
#include <QFile>

#include <items/item.h>

class ShopThread {
    Q_GADGET
public:
    ShopThread(const QString &id = QString())
        : _id(id) {
    }

    Q_PROPERTY(QString id MEMBER _id)
    Q_PROPERTY(QDateTime updated MEMBER _updated)
    Q_PROPERTY(QDateTime bumped MEMBER _bumped)

    QString content() const {
        return _content;
    }

    inline bool operator==(const ShopThread& other){
        return _id == other._id;
    }
    inline bool operator!=(const ShopThread& other) {
        return !(*this == other);
    }
private:
    QString _id;
    QDateTime _updated;
    QDateTime _bumped;
    QString _content;

    friend class Shop;
};

class ShopDataItem {
    Q_GADGET
public:
    Q_PROPERTY(QString id MEMBER _id)
    Q_PROPERTY(QString data MEMBER _data)
    Q_PROPERTY(QDateTime updated MEMBER _updated)
    inline bool operator==(const ShopDataItem& other){
        return _id == other._id;
    }
    inline bool operator!=(const ShopDataItem& other) {
        return !(*this == other);
    }

    QString getData() const {
        return _data;
    }

    enum Type {
        Item,
        Tab
    };

    virtual Type type() const = 0;
private:
    Type _type;
    QString _id;
    QString _data;
    QDateTime _updated;
    friend class Shop;
};

class ShopItem : public ShopDataItem {
    Q_GADGET
public:
    Q_PROPERTY(bool inherit MEMBER _inherit)
    Type type() const {
        return ShopDataItem::Item;
    }
private:
    bool _inherit;

    friend class Shop;
};

class ShopTab : public ShopDataItem {
    Q_GADGET
public:
    Type type() const {
        return ShopDataItem::Tab;
    }
private:
    friend class Shop;
};

typedef QMap<QString, ShopThread> ShopThreadList;
typedef QMap<QString, ShopItem> ShopItemMap;
typedef QMap<QString, ShopTab> ShopTabMap;

typedef std::function<const Item*(const QString &itemId)> ItemResolver;

class Shop : public QObject
{
    Q_OBJECT
public:
    explicit Shop(QObject *parent = 0);
    Shop(const QString &league, QObject *parent = 0);
    ~Shop() {}

    static qint64 dateToInt(QDateTime time) {
        return time.isNull() ? 0 : time.toMSecsSinceEpoch();
    }

    static QDateTime intToDate(qint64 time) {
        return time ? QDateTime::fromMSecsSinceEpoch(time) : QDateTime();
    }

    const QString league() const {
        return _league;
    }

    bool hasHistory() const {
        return false;
    }

    const QStringList threads() const {
        return _threads.uniqueKeys();
    }

    const QDateTime threadUpdated(const QString &id) const {
        return _threads.value(id, ShopThread(id))._updated;
    }

    const QDateTime threadBumped(const QString &id) const {
        return _threads.value(id, ShopThread(id))._bumped;
    }

    void addThread(const QString &id) {
        if (_threads.contains(id)) return;
        _threads.insert(id, ShopThread(id));
    }

    void removeThread(const QString &id) {
        _threads.remove(id);
    }

    Q_PROPERTY(QString league MEMBER _league)
    Q_PROPERTY(QString template MEMBER _template)
    Q_PROPERTY(QDateTime created MEMBER _created)

    inline void setTabData(const ShopTab &tab) {_tabs[tab._id] = tab;}
    Q_INVOKABLE inline void setTabData(const QString &id, const QString &data, QDateTime updated = QDateTime::currentDateTime()) {
        ShopTab tab;
        tab._id = id;
        tab._data = data;
        tab._updated = updated;
        setTabData(tab);
    }

    Q_INVOKABLE inline int clearTabData(const QString &id) {
        return _tabs.remove(id);
    }

    ShopTab getTabData(const QString &id) const {
        return _tabs.value(id);
    }


    inline void setItemData(const ShopItem &item) {_items[item._id] = item;}
    Q_INVOKABLE inline void setItemData(const QString &id, const QString &data, QDateTime updated = QDateTime::currentDateTime(), bool inherited = false) {
        ShopItem item;
        item._id = id;
        item._data = data;
        item._updated = updated;
        item._inherit = inherited;
        setItemData(item);
    }

    Q_INVOKABLE inline int clearItemData(const QString &id) {
        return _items.remove(id);
    }

    ShopItem getItemData(const QString &id) const {
        return _items.value(id);
    }

    void generateShopContent(const ItemResolver& resolver) {
        const int MaxThreadSize = 50000;

        QVariantHash data;
        data["item"] = QVariant::fromValue<Mustache::QtVariantContext::fn_t>([&resolver](const QString& val, Mustache::Renderer* renderer, Mustache::Context* context) -> QString {
            Q_UNUSED(renderer);
            Q_UNUSED(context);
            const Item* item = resolver(val);
            if (!item)
                return "[[NOITEM]]";
            return QString("[linkItem location=\"%1\" character=\"%2\" x=\"%3\" y=\"%4\"]")
                    .arg(item->data("inventoryId").toString())
                    .arg(item->data("league").toString())
                    .arg(item->data("x").toString())
                    .arg(item->data("y").toString());
//            // Stash
//            return "[linkItem location=\"{{inventoryId}}\" league=\"{{league}}\" x=\"{{x}}\" y=\"{{y}}\"]";
        });

        QHash<QString, QString> partials;
        partials["items"] = "{{#item}}1234567890{{/item}}{{#item}}1234567890{{/item}}{{#item}}1234567890{{/item}}{{#item}}1234567890{{/item}}";

        Mustache::PartialMap partialMap(partials);

        Mustache::QtVariantContext context(data, &partialMap);

        ShopTemplate renderer;
        const QString header = renderer.render("<", &context);
        const QString footer = renderer.render(">", &context);
        renderer.setMaxLength(50);
        renderer.resetPages();
        renderer.render("Hi {{>items}}", &context);

        qDebug() << header;
        qDebug() << renderer.getPages().join("\n-----\n");
        qDebug() << footer;
    }


    bool save(QJsonObject &object) const;
    bool load(const QJsonObject &object);
    Q_INVOKABLE inline QByteArray save() {
        QJsonObject object;
        save(object);
        QJsonDocument doc(object);
        return doc.toJson();
    }

    void setDisabled(bool disabled = true) {
        _disabled = disabled;
    }

    bool isDisabled() const {
        return _disabled;
    }

    void setUnused(bool unused = true) {
        _unused = unused;
    }

    bool isUnused() const {
        return _unused;
    }

    void clear() {
        _template = QString();
        _created = QDateTime();
        _threads.clear();
        _items.clear();
        _tabs.clear();

        _disabled = false;
        _unused = false;
    }
private:
    QString _league;
    QString _template;
    QDateTime _created;
    ShopThreadList _threads;
    ShopItemMap _items;
    ShopTabMap _tabs;

    bool _disabled;
    bool _unused;
};

typedef QMap<QString, Shop*> ShopList;

Q_DECLARE_METATYPE(Shop*)
Q_DECLARE_METATYPE(ShopThread*)
Q_DECLARE_METATYPE(ShopThread)
Q_DECLARE_METATYPE(ShopThreadList)
Q_DECLARE_METATYPE(ShopList)

#endif // SHOP_H
