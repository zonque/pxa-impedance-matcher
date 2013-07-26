#include "types.h"
#include "board.h"

extern __u32 _binary_input_zImage_start;
extern __u32 _binary_input_raumfeld_controller_0_dtb_start;
extern __u32 _binary_input_raumfeld_controller_1_dtb_start;
extern __u32 _binary_input_raumfeld_controller_2_dtb_start;
extern __u32 _binary_input_raumfeld_connector_0_dtb_start;
extern __u32 _binary_input_raumfeld_connector_1_dtb_start;
extern __u32 _binary_input_raumfeld_connector_2_dtb_start;
extern __u32 _binary_input_raumfeld_speaker_0_dtb_start;
extern __u32 _binary_input_raumfeld_speaker_1_dtb_start;
extern __u32 _binary_input_raumfeld_speaker_2_dtb_start;

static struct board boards[] = {
	/* Controller */
	{
		.machid		= 2413,
		.system_rev	= 0,
		.dtb		= &_binary_input_raumfeld_controller_0_dtb_start,
		.name		= "Raumfeld Controller, revision 0",
	},
	{
		.machid		= 2413,
		.system_rev	= 1,
		.dtb		= &_binary_input_raumfeld_controller_1_dtb_start,
		.name		= "Raumfeld Controller, revision 1",
	},
	{
		.machid		= 2413,
		.system_rev	= 2,
		.dtb		= &_binary_input_raumfeld_controller_2_dtb_start,
		.name		= "Raumfeld Controller, revision 2",
	},

	/* Controller */
	{
		.machid		= 2414,
		.system_rev	= 0,
		.dtb		= &_binary_input_raumfeld_connector_0_dtb_start,
		.name		= "Raumfeld Connector, revision 0",
	},
	{
		.machid		= 2414,
		.system_rev	= 1,
		.dtb		= &_binary_input_raumfeld_connector_1_dtb_start,
		.name		= "Raumfeld Connector, revision 1",
	},
	{
		.machid		= 2414,
		.system_rev	= 2,
		.dtb		= &_binary_input_raumfeld_connector_2_dtb_start,
		.name		= "Raumfeld Connector, revision 2",
	},

	/* Speaker */
	{
		.machid		= 2415,
		.system_rev	= 0,
		.dtb		= &_binary_input_raumfeld_speaker_0_dtb_start,
		.name		= "Raumfeld Speaker, revision 0",
	},
	{
		.machid		= 2415,
		.system_rev	= 1,
		.dtb		= &_binary_input_raumfeld_speaker_1_dtb_start,
		.name		= "Raumfeld Speaker, revision 1",
	},
	{
		.machid		= 2415,
		.system_rev	= 2,
		.dtb		= &_binary_input_raumfeld_speaker_2_dtb_start,
		.name		= "Raumfeld Speaker, revision 2",
	},
	{ 0, 0, NULL, NULL }	/* sentinel */
};

struct board *match_board(__u32 machid, __u32 revision)
{
	struct board *board;

	for (board = boards; board->machid; board++)
		if (board->machid == machid && board->system_rev == revision)
			return board;

	return NULL;
}

