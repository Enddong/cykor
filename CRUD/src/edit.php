<?php
include("includes/db.php");
include("templates/header.php");

if (!isset($_SESSION['userid'])) {
    header("Location: login.php");
    exit;
}

$id = $_GET['id'];

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $title = $_POST['title'];
    $content = $_POST['content'];

    mysqli_query($mydb, "UPDATE posts SET title='$title', content='$content' WHERE id=$id AND user_id=" . $_SESSION['userid']);
    header("Location: post.php?id=$id");
    exit;
}

$result = mysqli_query($mydb, "SELECT * FROM posts WHERE id=$id AND user_id=" . $_SESSION['userid']);
$post = mysqli_fetch_assoc($result);
?>

<h2>글 수정</h2>
<form method="POST" action="edit.php?id=<?php echo $id; ?>">
    제목: <input type="text" name="title" value="<?php echo $post['title']; ?>"><br><br>
    내용:<br>
    <textarea name="content" rows="10" cols="50"><?php echo $post['content']; ?></textarea><br><br>
    <input type="submit" value="수정하기">
</form>

<?php include("templates/footer.php"); ?>
