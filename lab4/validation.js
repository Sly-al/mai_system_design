// Включить/обновить валидацию коллекции hotels и проверить отказ на невалидный документ
// mongosh mongodb://127.0.0.1:27017/hotel_booking validation.js

const spec = {
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
};

try {
  db.runCommand({ collMod: 'hotels', validator: spec, validationLevel: 'strict', validationAction: 'error' });
  print('collMod hotels: OK');
} catch (e) {
  print('collMod failed');
  print(e);
}

print('Пробуем невалидную вставку - ожидаем  ошибку:');
try {
  db.hotels.insertOne({
    _id: NumberLong(99999),
    owner_user_id: NumberLong(1),
    name: '',
    city: 'X',
    address: 'Y',
  });
  print('ERROR');
} catch (e) {
  print(e.message);
}
