
import welcomeImg from '../img/welcome.png';

function Home() {
  return (
    <div className="container">
      <h2><strong>환영합니다!</strong></h2>
      <p>자유롭게 게시판을 이용해주세요</p>
      <img src={welcomeImg} alt="농담곰" style={{ width: '200px' }} />
    </div>
  );
}

export default Home;