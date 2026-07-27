class_name TimeEffects

static func hitstop(tree :SceneTree, time: float = 1.0)-> void:
	tree.paused = true;
	await tree.create_timer(time, true, false, false).timeout #maybe should be true lol
	tree.paused = false;
