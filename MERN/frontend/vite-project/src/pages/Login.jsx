import { useState } from 'react';
import axios from '../api/axios';

function Login({ onLogin }) {
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');

  const handleLogin = async (e) => {
    e.preventDefault();
    try {
      const res = await axios.post('/users/login', { username, password });
      alert('로그인 성공!');
      onLogin(res.data);
    } catch (err) {
      alert('로그인 실패');
    }
  };

  return (
    <form onSubmit={handleLogin}>
      <h2>로그인</h2>
      <input placeholder="아이디" value={username} onChange={(e) => setUsername(e.target.value)} />
      <input type="password" placeholder="비밀번호" value={password} onChange={(e) => setPassword(e.target.value)} />
      <button type="submit">로그인</button>
    </form>
  );
}

export default Login;
