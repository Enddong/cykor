const User = require('../models/User');

exports.register = async (req, res) => {
  const { username, password } = req.body;

  const existing = await User.findOne({ username });
  if (existing) return res.status(400).json({ message: '존재하는 사용자입니다' });

  const newUser = new User({ username, password });
  await newUser.save();
  res.json({ message: '회원가입 성공' });
};

exports.login = async (req, res) => {
  const { username, password } = req.body;

  const user = await User.findOne({ username, password });
  if (!user) return res.status(401).json({ message: '로그인 실패' });

  res.json({ message: '로그인 성공', userId: user._id, username: user.username });
};
