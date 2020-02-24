<!-- Logout Modal-->
<div class="modal fade" id="logoutModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel" aria-hidden="true">
	<div class="modal-dialog" role="document">
		<div class="modal-content">
			<div class="modal-header">
				<h5 class="modal-title" id="exampleModalLabel">Do you want to Logout?</h5>
				<button class="close" type="button" data-dismiss="modal" aria-label="Close">
					<span aria-hidden="true"><img src="./img/favicon.ico" width="30px;"></span>
				</button>
			</div>
			<div class="modal-body">Select "Logout" below if you are ready to end your current session.</div>
			<div class="modal-footer">
				<button class="btn btn-secondary" type="button" data-dismiss="modal">Cancel</button>
				<a class="btn btn-primary" href="logout">Logout</a>
			</div>
		</div>
	</div>
</div>

<!-- Confirm Modal-->
<div class="modal fade" id="confirmModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel" aria-hidden="true">
	<div class="modal-dialog" role="document">
		<div class="modal-content">
			<div class="modal-header">
				<h5 class="modal-title" id="modal-title">Modal title</h4>
					<button class="close" type="button" data-dismiss="modal" aria-label="Close">
						<span aria-hidden="true"><img src="./img/favicon.ico" width="30px;"></span>
					</button>
			</div>
			<div class="modal-body" id="modal-body">
			</div>
			<div name="modal-btn-value" id="modal-btn-value" hidden></div>
			<div name="modal-data-value" id="modal-data-value" hidden></div>
			<div name="modal-api-value" id="modal-api-value" hidden></div>
			<div class="modal-footer">
				<button type="button" id="modal-cancle" class="btn btn-secondary" data-dismiss="modal">Cancle</button>
				<button type="button" id="modal-confirm" class="btn btn-primary" data-dismiss="modal">Okay</button>
			</div>
		</div><!-- /.modal-content -->
	</div><!-- /.modal-dialog -->
</div><!-- /.modal -->