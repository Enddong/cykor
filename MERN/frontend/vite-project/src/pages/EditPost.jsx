import { useState, useEffect } from 'react';
import axios from '../api/axios';
import { useParams, useNavigate } from 'react-router-dom';

function EditPost() {
  const { id } = useParams();
  const navigate = useNavigate();
  const [title, setTitle] = useState('');
  const [content, setContent] = useState('');

  useEffect(() => {
    axios.get(`/posts/${id}`).then((res) => {
      setTitle(res.data.title);
      setContent(res.data.content);
    });
  }, [id]);

  const handleUpdate = async (e) => {
    e.preventDefault();
    await axios.put(`/posts/${id}`, { title, content });
    alert('수정 완료!');
    navigate('/');
  };

  return (
    <form onSubmit={handleUpdate}>
      <h2>글 수정</h2>
      <input value={title} onChange={(e) => setTitle(e.target.value)} />
      <textarea value={content} onChange={(e) => setContent(e.target.value)} />
      <button type="submit">수정하기</button>
    </form>
  );
}

export default EditPost;
