const Post = require('../models/Post');

exports.getPosts = async (req, res) => {
  const posts = await Post.find().populate('user', 'username').sort({ writetime: -1 });
  res.json(posts);
};

exports.getPostById = async (req, res) => {
  const post = await Post.findById(req.params.id).populate('user', 'username');
  res.json(post);
};

exports.createPost = async (req, res) => {
  const { title, content, userId } = req.body;
  const newPost = new Post({ title, content, user: userId });
  await newPost.save();
  res.json(newPost);
};

exports.updatePost = async (req, res) => {
  const { title, content } = req.body;
  const updated = await Post.findByIdAndUpdate(
    req.params.id,
    { title, content },
    { new: true }
  );
  res.json(updated);
};

exports.deletePost = async (req, res) => {
  await Post.findByIdAndDelete(req.params.id);
  res.json({ message: '삭제 완료' });
};
