import { Link } from 'react-router-dom';

function Header({ user, onLogout, onLoginClick, onRegisterClick }) {
  return (
    <header>
      <nav>
        <Link to="/">홈</Link> | <Link to="/board">게시판</Link>
        {!user ? (
          <>
            {' '}| <a href="#" onClick={(e) => { e.preventDefault(); onLoginClick(); }}>로그인</a>
            {' '}| <a href="#" onClick={(e) => { e.preventDefault(); onRegisterClick(); }}>회원가입</a>
          </>
        ) : (
          <>
            {' '}| <span>{user.username}</span>
            <button onClick={onLogout}>로그아웃</button>
          </>
        )}
      </nav>
    </header>
  );
}

export default Header;
