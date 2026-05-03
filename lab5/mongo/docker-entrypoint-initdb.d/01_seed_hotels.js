// Инициализация MongoDB: коллекции hotels и counters, валидация, тестовые данные.

db = db.getSiblingDB('hotel_booking');

db.createCollection('hotels', {
  validator: {
    $jsonSchema: {
      bsonType: 'object',
      required: ['_id', 'owner_user_id', 'name', 'city', 'address'],
      properties: {
        _id: { bsonType: 'long', minimum: 1 },
        owner_user_id: { bsonType: 'long', minimum: 1 },
        name: { bsonType: 'string', minLength: 1, maxLength: 200 },
        city: { bsonType: 'string', minLength: 1, maxLength: 120 },
        address: { bsonType: 'string', minLength: 1, maxLength: 300 },
      },
    },
  },
  validationLevel: 'strict',
  validationAction: 'error',
});

db.hotels.createIndex({ city: 1, _id: 1 });

const hotels = [
  {_id: NumberLong(1), owner_user_id: NumberLong(1), name: 'Grand Moscow', city: 'Moscow', address: 'Tverskaya 1'},
  {_id: NumberLong(2), owner_user_id: NumberLong(2), name: 'Nevsky Inn', city: 'Saint Petersburg', address: 'Nevsky 10'},
  {_id: NumberLong(3), owner_user_id: NumberLong(3), name: 'Ural Lodge', city: 'Ekaterinburg', address: 'Lenina 5'},
  {_id: NumberLong(4), owner_user_id: NumberLong(1), name: 'Kremlin View', city: 'Moscow', address: 'Varvarka 3'},
  {_id: NumberLong(5), owner_user_id: NumberLong(4), name: 'Baikal Shore', city: 'Irkutsk', address: 'Gagarin 7'},
  {_id: NumberLong(6), owner_user_id: NumberLong(5), name: 'Volga Plaza', city: 'Kazan', address: 'Bauman 12'},
  {_id: NumberLong(7), owner_user_id: NumberLong(6), name: 'Siberian Star', city: 'Novosibirsk', address: 'Krasny 20'},
  {_id: NumberLong(8), owner_user_id: NumberLong(7), name: 'Golden Ring', city: 'Yaroslavl', address: 'Pervomayskaya 4'},
  {_id: NumberLong(9), owner_user_id: NumberLong(8), name: 'Black Sea', city: 'Sochi', address: 'Kurortny 9'},
  {_id: NumberLong(10), owner_user_id: NumberLong(9), name: 'Caspian', city: 'Astrakhan', address: 'Admiralteyskaya 2'},
  {_id: NumberLong(11), owner_user_id: NumberLong(10), name: 'Pacific', city: 'Vladivostok', address: 'Svetlanskaya 15'},
  {_id: NumberLong(12), owner_user_id: NumberLong(11), name: 'Arctic', city: 'Murmansk', address: 'Lenina 30'},
];

db.hotels.insertMany(hotels);

db.createCollection('counters');
db.counters.insertOne({_id: 'hotel_seq', seq: NumberLong(12)});
