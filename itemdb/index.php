<?php

  include('inc_config.php');

  function list_items() {
    $db = new PDO(DBCONNECT, RW_USER, RW_PASS);
    $sql = '
select
items.id,
items.name proper_name,
prefix,
base_items.name base_name,
suffix,
(base_items.level + item_mods.level) level,
quality,
type,
icon32,
icon64,
(base_items.dmg_min + item_mods.dmg_min) dmg_min,
(base_items.dmg_max + item_mods.dmg_max) dmg_max,
(base_items.abs_min + item_mods.abs_min) abs_min,
(base_items.abs_max + item_mods.abs_max) abs_max,
effect,
req_stat,
req_val,
bonus_stat,
bonus_pval,
bonus_mval,
bonus_oval,
bonus_dval,
bonus_art,
sfx,
gfx,
loot
from (items inner join base_items on items.base_type = base_items.id)
inner join item_mods on items.item_mod = item_mods.id
order by items.id
';

    $stmt = $db->prepare($sql);
    $stmt->execute();
    while ($row = $stmt->fetch()) {
	echo "<tr>\n";

        $name = "";
        if (!is_null($row["proper_name"]))
          $name = $row["proper_name"];
        else {
          if (!is_null($row["prefix"]))
            $name = $row["prefix"] . $row["base_name"];
          else
            $name = $row["base_name"] . $row["suffix"];
        }

	echo "<td>" . $row["id"] . "</td>\n";
	echo "<td>" . $name . "</td>\n";
	echo "<td>" . $row["level"] . "</td>\n";
	echo "<td>" . $row["quality"] . "</td>\n";
	echo "<td>" . $row["type"] . "</td>\n";
        if ($row["icon64"] != NULL) {
	  echo "<td>" . $row["icon32"] . "," . $row["icon64"] . "</td>\n";
	}
        else {
	  echo "<td>" . $row["icon32"] . "</td>\n";
        }
	echo "<td>" . $row["dmg_min"] . "," . $row["dmg_max"] . "</td>\n";
	echo "<td>" . $row["abs_min"] . "," . $row["abs_max"] . "</td>\n";
	echo "<td>" . $row["req_stat"] . "," . $row["req_val"] . "</td>\n";
	echo "<td>" . $row["bonus_stat"] . ",";
	if ($row["req_stat"] == "p")
	  echo $row["bonus_pval"];
	else if ($row["req_stat"] == "m")
	  echo $row["bonus_mval"];
	else if ($row["req_stat"] == "o")
	  echo $row["bonus_oval"];
	else if ($row["req_stat"] == "d")
	  echo $row["bonus_dval"];
	else
	  echo $row["bonus_art"];

	echo "</td>\n";
	echo "<td>" . $row["sfx"] . "</td>\n";
	echo "<td>" . $row["gfx"] . "</td>\n";
	echo "<td>" . $row["loot"] . "</td>\n";

	echo "</tr>\n";
    }
    $stmt = null;
    $db = null;
  }
?><html>
<head>
  <style>
table {border-collapse: collapse;}
table, th, td {border: solid 1px #666; font-size: 9pt; font-family: sans-serif;}

  </style>
</head>
<body>
<table>
  <tr>
    <th>id</th>
    <th>name</th>
    <th>level</th>
    <th>quality</th>
    <th>type</th>
    <th>icon</th>
    <th>dmg</th>
    <th>abs</th>
    <th>req</th>
    <th>bonus</th>
    <th>sfx</th>
    <th>gfx</th>
    <th>loot</th>
  </tr>
<?php list_items(); ?>
</table>
</body>
</html>
