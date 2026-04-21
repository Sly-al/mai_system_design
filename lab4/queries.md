# Примеры запросов MongoDB

## Создание

```javascript
db.hotels.insertOne({
  _id: NumberLong(100),
  owner_user_id: NumberLong(1),
  name: "Test Inn",
  city: "Moscow",
  address: "Test street 1"
});
```

## Чтение

```javascript
db.hotels.find({ city: "Moscow" });

db.hotels.find({ city: { $in: ["Moscow", "Sochi"] } }).sort({ _id: 1 });

db.hotels.find({ _id: { $gt: NumberLong(3), $lt: NumberLong(8) } });

db.hotels.find({
  $and: [
    { city: { $ne: "Moscow" } },
    { owner_user_id: { $lte: NumberLong(5) } }
  ]
});

db.hotels.find({
  $or: [{ name: /Grand/ }, { city: /^S/ }]
});
```

## Обновление

```javascript
db.hotels.updateOne(
  { _id: NumberLong(1) },
  { $set: { address: "Tverskaya 1 (updated)" } }
);

db.counters.updateOne(
  { _id: "hotel_seq" },
  { $inc: { seq: NumberLong(1) } },
  { upsert: true }
);
```

## Delete

```javascript
db.hotels.deleteOne({ _id: NumberLong(100) });
```

## Агрегация

Число отелей по городам:

```javascript
db.hotels.aggregate([
  { $match: { owner_user_id: { $gte: NumberLong(1) } } },
  { $group: { _id: "$city", total: { $sum: 1 } } },
  { $sort: { total: -1 } },
  { $project: { _id: 0, city: "$_id", hotels: "$total" } }
]);
```
