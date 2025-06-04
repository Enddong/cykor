require('dotenv').config();
const express = require('express');
const mongoose = require('mongoose');
const cors = require('cors');

const userRoutes = require('./routes/userRoutes');
const postRoutes = require('./routes/postRoutes');

const app = express();
app.use(cors());
app.use(express.json());

const connectWithRetry = () => {
  mongoose.connect(process.env.MONGO_URI)
    .then(() => console.log('MongoDB 연결 성공'))
    .catch((err) => {
      console.error('MongoDB 연결 실패. 재시도:', err.message);
      setTimeout(connectWithRetry, 5000);
    });
};
connectWithRetry();

app.use('/api/users', userRoutes);
app.use('/api/posts', postRoutes);

const PORT = process.env.PORT || 5000;
app.listen(PORT, () => {
  console.log(` http://localhost:${PORT}`);
});
