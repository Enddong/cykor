<?php
include("includes/db.php");
include("templates/header.php");

if (isset($_POST['submit'])) {
    $username = $_POST['username'];
    $password = $_POST['password'];
    $age = $_POST['age'];

    if ($username == "" || $password == "" || $age == "") {
        echo "빈칸 없이 전부 입력해주세요.<br/>";
        echo "<a href='register.php'>돌아가기</a>";
    } else {
        $result = mysqli_query($mydb, "INSERT INTO users (username, password, age) VALUES ('$username', '$password', $age)");

        if ($result) {
            $userId = mysqli_insert_id($mydb);
            echo "회원가입이 완료되었습니다!<br/>";
            echo "당신의 사용자 ID는 <strong>$userId</strong> 입니다.<br/>";
            echo "<a href='login.php'>로그인하기</a>";
        } else {
            echo "회원가입 중 오류가 발생했습니다.";
        }
    }
} else {
?>

<h2>회원가입</h2>
<form method="POST" action="register.php">
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
            <td>나이</td>
            <td><input type="number" name="age"></td>
        </tr>
        <tr>
            <td></td>
            <td><input type="submit" name="submit" value="가입하기"></td>
        </tr>
    </table>
</form>

<?php
}
include("templates/footer.php");
?>
