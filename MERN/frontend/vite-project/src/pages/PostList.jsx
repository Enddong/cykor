import { useState, useEffect } from 'react';
import axios from '../api/axios';
import { useNavigate } from 'react-router-dom';

function PostList({ user }) {
  const [posts, setPosts] = useState([]);
  const navigate = useNavigate();

  useEffect(() => {
    axios.get('/posts')
      .then((res) => setPosts(res.data))
      .catch((err) => console.error('게시글 불러오기 실패:', err));
  }, []);

  const handleDelete = async (id) => {
    if (window.confirm('정말 삭제하시겠습니까?')) {
      try {
        await axios.delete(`/posts/${id}`);
        setPosts(posts.filter((post) => post._id !== id));
      } catch (err) {
        console.error('삭제 실패:', err);
      }
    }
  };

  return (
    <div>
      <h3>게시글 목록</h3>
      <table>
        <thead>
          <tr>
            <th>제목</th>
            <th>작성자</th>
            <th>작성일</th>
            {user && <th>관리</th>}
          </tr>
        </thead>
        <tbody>
          {posts.map((post) => (
            <tr key={post._id}>
              <td>{post.title}</td>
              <td>{post.user?.username || '알 수 없음'}</td>
              <td>{new Date(post.writetime).toLocaleString()}</td>
              {user && user.userId === post.user?._id ? (
                <td>
                  <button onClick={() => navigate(`/edit/${post._id}`)}>수정</button>
                  <button onClick={() => handleDelete(post._id)}>삭제</button>
                </td>
              ) : user ? <td></td> : null}
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}

export default PostList;
