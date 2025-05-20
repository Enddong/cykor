<?php
include("includes/db.php");
include("templates/header.php");

$postId = $_GET['id'] ?? null;

if (!$postId || !is_numeric($postId)) {
    echo "잘못된 접근입니다.";
} else {
    $query = "
        SELECT posts.*, users.username
        FROM posts
        JOIN users ON posts.user_id = users.id
        WHERE posts.id = $postId
    ";
    $result = mysqli_query($mydb, $query);
    $post = mysqli_fetch_assoc($result);

    if (!$post) {
        echo "해당 글이 존재하지 않습니다.";
    } else {
?>
        <h2><?php echo htmlspecialchars($post['title']); ?></h2>
        <p><strong>작성자:</strong> <?php echo htmlspecialchars($post['username']); ?></p>
        <p><strong>작성일:</strong> <?php echo $post['writetime']; ?></p>
        <hr>
        <div>
            <?php echo nl2br(htmlspecialchars($post['content'])); ?>
        </div>
<?php
    }
}
?>

<?php include("templates/footer.php"); ?>
