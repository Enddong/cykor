import { useState } from 'react';
import axios from '../api/axios';

function WritePost({ user }) {
  const [title, setTitle] = useState('');
  const [content, setContent] = useState('');

  const handleSubmit = async (e) => {
    e.preventDefault();
    try {
      await axios.post('/posts', {
        title,
        content,
        userId: user.userId,
      });
      alert('글 작성 완료!');
      setTitle('');
      setContent('');
    } catch (err) {
      alert('작성 실패');
    }
  };

  return (
    <form onSubmit={handleSubmit}>
      <h2>글 작성</h2>
      <input placeholder="제목" value={title} onChange={(e) => setTitle(e.target.value)} />
      <textarea placeholder="내용" value={content} onChange={(e) => setContent(e.target.value)} />
      <button type="submit">작성</button>
    </form>
  );
}

export default WritePost;
