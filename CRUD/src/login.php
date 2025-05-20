<?php
include("includes/db.php");
include("templates/header.php");

if (isset($_POST['submit'])) {
    $username = $_POST['username'];
    $password = $_POST['password'];

    if ($username == "" || $password == "") {
        echo "아이디와 비밀번호 모두 입력해야 합니다.<br/>";
        echo "<a href='login.php'>돌아가기</a>";
    } else {
        $result = mysqli_query($mydb, "SELECT * FROM users WHERE username='$username' AND password='$password'");
        $row = mysqli_fetch_assoc($result);

        if ($row) {
            $_SESSION['userid'] = $row['id'];
            $_SESSION['username'] = $row['username'];
            header("Location: index.php");
            exit;
        } else {
            echo "이름 또는 비밀번호가 틀렸습니다.<br/>";
            echo "<a href='login.php'>돌아가기</a>";
        }
    }
} else {
?>

<h2>로그인</h2>
<form method="POST" action="login.php">
    <table>
        <tr>
            <td>이름</td>
            <td><input type="text" name="username"></td>
        </tr>
        <tr>
            <td>비밀번호</td>
            <td><input type="password" name="password"></td>
        </tr>
        <tr>
            <td></td>
            <td><input type="submit" name="submit" value="로그인"></td>
        </tr>
    </table>
</form>

<?php
}
include("templates/footer.php");
?>
