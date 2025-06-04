import { useState } from 'react';
import WritePost from '../pages/WritePost';
import PostList from '../pages/PostList';
import Login from '../pages/Login';
import Register from '../pages/Register';

function BoardPage({ user, onLogin, view }) {
  const [showWrite, setShowWrite] = useState(false);

  const toggleWriteForm = () => {
    setShowWrite((prev) => !prev);
  };

  return (
    <div className="container">
      <h2>게시판</h2>
      <hr />

      {!user && (
        <>
          {view === 'register' && <Register />}
          {view === 'login' && <Login onLogin={onLogin} />}
        </>
      )}

      {user && (
        <button onClick={toggleWriteForm}>
          {showWrite ? '닫기' : '글 작성'}
        </button>
      )}

      {showWrite && user && <WritePost user={user} />}

      <PostList user={user} />
    </div>
  );
}

export default BoardPage;
